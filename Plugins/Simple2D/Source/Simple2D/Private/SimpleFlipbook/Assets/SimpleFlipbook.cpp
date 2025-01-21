// "Unreal Pokémon" created by Retro & Chill.


#include "SimpleFlipbook/Assets/SimpleFlipbook.h"
#include "BitmapUtils.h"
#include "GeomTools.h"
#include "Paper2DModule.h"
#include "PaperSprite.h"

struct FBoundaryImage
{
    TArray<int8> Pixels;

    // Value to return out of bounds
    int8 OutOfBoundsValue;

    int32 X0;
    int32 Y0;
    int32 Width;
    int32 Height;

    FBoundaryImage(const FIntPoint& Pos, const FIntPoint& Size)
    {
        OutOfBoundsValue = 0;

        X0 = Pos.X - 1;
        Y0 = Pos.Y - 1;
        Width = Size.X + 2;
        Height = Size.Y + 2;

        Pixels.AddZeroed(Width * Height);
    }

    int32 GetIndex(int32 X, int32 Y) const
    {
        const int32 LocalX = X - X0;
        const int32 LocalY = Y - Y0;

        if ((LocalX >= 0) && (LocalX < Width) && (LocalY >= 0) && (LocalY < Height))
        {
            return LocalX + (LocalY * Width);
        }
        else
        {
            return INDEX_NONE;
        }
    }

    int8 GetPixel(int32 X, int32 Y) const
    {
        const int32 Index = GetIndex(X, Y);
        if (Index != INDEX_NONE)
        {
            return Pixels[Index];
        }
        else
        {
            return OutOfBoundsValue;
        }
    }

    void SetPixel(int32 X, int32 Y, int8 Value)
    {
        const int32 Index = GetIndex(X, Y);
        if (Index != INDEX_NONE)
        {
            Pixels[Index] = Value;
        }
    }
};

// Get a divisor ("pixel" size) from the "detail" parameter
// Size is fed in for possible changes later
static int32 GetDivisorFromDetail(const FIntPoint& Size, float Detail)
{
    //@TODO: Consider MaxSize somehow when deciding divisor
    //int32 MaxSize = FMath::Max(Size.X, Size.Y);
    return  FMath::Lerp(8, 1, FMath::Clamp(Detail, 0.0f, 1.0f));
}

static void TraceContour(TArray<FIntPoint>& Result, const TArray<FIntPoint>& Points)
{
	const int PointCount = Points.Num();
	if (PointCount < 2)
	{
		return;
	}

	int CurrentX = (int)Points[0].X;
	int CurrentY = (int)Points[0].Y;
	int CurrentDirection = 0;
	int FirstDx = (int)Points[1].X - CurrentX;
	int FirstDy = (int)Points[1].Y - CurrentY;
	
	if (FirstDx == 1 && FirstDy == 0) CurrentDirection = 0;
	else if (FirstDx == 1 && FirstDy == 1) CurrentDirection = 1;
	else if (FirstDx == 0 && FirstDy == 1) CurrentDirection = 1;
	else if (FirstDx == -1 && FirstDy == 1) CurrentDirection = 2;
	else if (FirstDx == -1 && FirstDy == 0) CurrentDirection = 2;
	else if (FirstDx == -1 && FirstDy == -1) CurrentDirection = 3;
	else if (FirstDx == 0 && FirstDy == -1) CurrentDirection = 3;
	else if (FirstDx == 1 && FirstDy == -1) CurrentDirection = 0;

	int CurrentPointIndex = 0;

	const int StartX = CurrentX;
	const int StartY = CurrentY;
	const int StartDirection = CurrentDirection;

	static const int DirectionDx[] = { 1, 0, -1, 0 };
	static const int DirectionDy[] = { 0, 1, 0, -1 };

	bool bFinished = false;
	while (!bFinished)
	{
		const FIntPoint& NextPoint = Points[(CurrentPointIndex + 1) % PointCount];
		const int NextDx = (int)NextPoint.X - CurrentX;
		const int NextDy = (int)NextPoint.Y - CurrentY;

		int LeftDirection = (CurrentDirection + 3) % 4;
		int CurrentDx = DirectionDx[CurrentDirection];
		int CurrentDy = DirectionDy[CurrentDirection];
		int LeftDx = DirectionDx[LeftDirection];
		int LeftDy = DirectionDy[LeftDirection];
		bool bDidMove = true;
		if (NextDx != 0 || NextDy != 0)
		{
			if (NextDx == LeftDx && NextDy == LeftDy)
			{
				// Space to the left, turn left and move forwards
				CurrentDirection = LeftDirection;
				CurrentX += LeftDx;
				CurrentY += LeftDy;
			}
			else
			{
				// Wall to the left. Add the corner vertex to our output.
				Result.Add(FIntPoint((int)((float)CurrentX + 0.5f + (float)(CurrentDx + LeftDx) * 0.5f), (int)((float)CurrentY + 0.5f + (float)(CurrentDy + LeftDy) * 0.5f)));
				if (NextDx == CurrentDx && NextDy == CurrentDy)
				{
					// Move forward
					CurrentX += CurrentDx;
					CurrentY += CurrentDy;
				}
				else if (NextDx == CurrentDx + LeftDx && NextDy == CurrentDy + LeftDy)
				{
					// Move forward, turn left, move forwards again
					CurrentX += CurrentDx;
					CurrentY += CurrentDy;
					CurrentDirection = LeftDirection;
					CurrentX += LeftDx;
					CurrentY += LeftDy;
				}
				else
				{
					// Turn right
					CurrentDirection = (CurrentDirection + 1) % 4;
					bDidMove = false;
				}
			}
		}
		if (bDidMove)
		{
			++CurrentPointIndex;
		}

		if (CurrentX == StartX && CurrentY == StartY && CurrentDirection == StartDirection)
		{
			bFinished = true;
		}
	}
}

void RemoveCollinearPoints(TArray<FIntPoint>& PointList)
{
    if (PointList.Num() < 3)
    {
        return;
    }
	
    // Wrap around to get the final pair of vertices (N-1, 0, 1)
    for (int32 VertexIndex = 1; VertexIndex <= PointList.Num() && PointList.Num() >= 3; )
    {
        const FVector2D A(PointList[VertexIndex-1]);
        const FVector2D B(PointList[VertexIndex % PointList.Num()]);
        const FVector2D C(PointList[(VertexIndex+1) % PointList.Num()]);

        // Determine if the area of the triangle ABC is zero (if so, they're collinear)
        const float AreaABC = (A.X * (B.Y - C.Y)) + (B.X * (C.Y - A.Y)) + (C.X * (A.Y - B.Y));

        if (FMath::Abs(AreaABC) < KINDA_SMALL_NUMBER)
        {
            // Remove B
            PointList.RemoveAt(VertexIndex % PointList.Num());
        }
        else
        {
            // Continue onwards
            ++VertexIndex;
        }
    }
}


static void BruteForceSimplifier(TArray<FIntPoint>& Points, float Epsilon)
{
	float FlatEdgeDistanceThreshold = (int)(Epsilon * Epsilon);

	// Run through twice to remove remnants from staircase artifacts
	for (int Pass = 0; Pass < 2; ++Pass)
	{
		for (int I = 0; I < Points.Num() && Points.Num() > 3; ++I)
		{
			int StartRemoveIndex = (I + 1) % Points.Num();
			int EndRemoveIndex = StartRemoveIndex;
			FIntPoint& A = Points[I];
			// Keep searching to find if any of the vector rejections fail in subsequent points on the polygon
			// A B C D E F (eg. when testing A B C, test rejection for BA, CA)
			// When testing A E F, test rejection for AB-AF, AC-AF, AD-AF, AE-AF
			// When one of these fails we discard all verts between A and one before the current vertex being tested
			for (int J = I; J < Points.Num(); ++J)
			{
				int IndexC = (J + 2) % Points.Num();
				FIntPoint& C = Points[IndexC];
				bool bSmallOffsetFailed = false;

				for (int K = I; K <= J && !bSmallOffsetFailed; ++K)
				{
					int IndexB = (K + 1) % Points.Num();
					FIntPoint& B = Points[IndexB];

					FVector2D CA = C - A;
					FVector2D BA = B - A;
					FVector2D Rejection_BA_CA = BA - (FVector2D::DotProduct(BA, CA) / FVector2D::DotProduct(CA, CA)) * CA;
					float RejectionLengthSquared = Rejection_BA_CA.SizeSquared();
					// If any of the points is behind the polyline up till now, it gets rejected. Staircase artefacts are handled in a second pass
					if (RejectionLengthSquared > FlatEdgeDistanceThreshold || FVector2D::CrossProduct(CA, BA) < 0) 
					{
						bSmallOffsetFailed = true;
						break;
					}
				}

				if (bSmallOffsetFailed)
				{
					break;
				}
				else
				{
					EndRemoveIndex = (EndRemoveIndex + 1) % Points.Num();
				}
			}

			// Remove the vertices that we deemed "too flat"
			if (EndRemoveIndex > StartRemoveIndex)
			{
				Points.RemoveAt(StartRemoveIndex, EndRemoveIndex - StartRemoveIndex);
			}
			else if (EndRemoveIndex < StartRemoveIndex)
			{
				Points.RemoveAt(StartRemoveIndex, Points.Num() - StartRemoveIndex);
				Points.RemoveAt(0, EndRemoveIndex);
				// The search has wrapped around, no more vertices to test
				break;
			}
		}
	} // Pass
}

void SimplifyPoints(TArray<FIntPoint>& Points, float Epsilon)
{
    //	FDouglasPeuckerSimplifier Simplifier(Points, Epsilon);
    //	Simplifier.Execute(Points);
    BruteForceSimplifier(Points, Epsilon);
}

USimpleFlipbook::USimpleFlipbook() {
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaskedMaterialRef(TEXT("/Simple2D/Materials/MaskedUnlitFlipbookMaterial"));
    DefaultMaterial = MaskedMaterialRef.Object;
}

int32 USimpleFlipbook::GetNumFrames() const {
    int32 SumFrames = 0;
    for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex)
    {
        SumFrames += KeyFrames[KeyFrameIndex].FrameRun;
    }

    return SumFrames;
}

float USimpleFlipbook::GetTotalDuration() const {
    if (FramesPerSecond != 0) {
        return static_cast<float>(GetNumFrames()) / FramesPerSecond;
    }

    return 0.0f;
}

int32 USimpleFlipbook::GetKeyFrameIndexAtTime(float Time, bool bClampToEnds) const {
    if (Time < 0.0f && !bClampToEnds)
    {
        return INDEX_NONE;
    }

    if (FramesPerSecond > 0.0f)
    {
        float SumTime = 0.0f;

        for (int32 KeyFrameIndex = 0; KeyFrameIndex < KeyFrames.Num(); ++KeyFrameIndex)
        {
            SumTime += KeyFrames[KeyFrameIndex].FrameRun / FramesPerSecond;

            if (Time <= SumTime)
            {
                return KeyFrameIndex;
            }
        }

        // Return the last frame (note: relies on INDEX_NONE = -1 if there are no key frames)
        return KeyFrames.Num() - 1;
    }
    
    return KeyFrames.Num() > 0 ? 0 : INDEX_NONE;
}

FBoxSphereBounds USimpleFlipbook::GetRenderBounds() const {
    FBox BoundingBox(ForceInit);
	
    for (int32 VertexIndex = 0; VertexIndex < BakedRenderData.Num(); ++VertexIndex)
    {
        const FVector4& VertXYUV = BakedRenderData[VertexIndex];
        const FVector Vert((PaperAxisX * VertXYUV.X) + (PaperAxisY * VertXYUV.Y));
        BoundingBox += Vert;
    }
	
    // Make the whole thing a single unit 'deep'
    const FVector HalfThicknessVector = 0.5f * PaperAxisZ;
    BoundingBox += -HalfThicknessVector;
    BoundingBox += HalfThicknessVector;

    return FBoxSphereBounds(BoundingBox);
}

#if WITH_EDITOR
FVector2D USimpleFlipbook::GetPivotPosition() const {
    FVector2D RawPivot = GetRawPivotPosition();

    if (bSnapPivotToPixelGrid)
    {
        RawPivot.X = FMath::RoundToFloat(RawPivot.X);
        RawPivot.Y = FMath::RoundToFloat(RawPivot.Y);
    }

    return RawPivot;
}

FVector2D USimpleFlipbook::GetRawPivotPosition() const {
    FVector2D TopLeftUV = FVector2D::Zero();
    FVector2D Dimension = FVector2D(GetSourceTexture()->GetSizeX(), GetSourceTexture()->GetSizeY());
    
    switch (PivotMode)
    {
    case ESpritePivotMode::Top_Left:
        return TopLeftUV;
    case ESpritePivotMode::Top_Center:
        return FVector2D(TopLeftUV.X + Dimension.X * 0.5f, TopLeftUV.Y);
    case ESpritePivotMode::Top_Right:
        return FVector2D(TopLeftUV.X + Dimension.X, TopLeftUV.Y);
    case ESpritePivotMode::Center_Left:
        return FVector2D(TopLeftUV.X, TopLeftUV.Y + Dimension.Y * 0.5f);
    case ESpritePivotMode::Center_Center:
        return FVector2D(TopLeftUV.X + Dimension.X * 0.5f, TopLeftUV.Y + Dimension.Y * 0.5f);
    case ESpritePivotMode::Center_Right:
        return FVector2D(TopLeftUV.X + Dimension.X, TopLeftUV.Y + Dimension.Y * 0.5f);
    case ESpritePivotMode::Bottom_Left:
        return FVector2D(TopLeftUV.X, TopLeftUV.Y + Dimension.Y);
    case ESpritePivotMode::Bottom_Center:
        return FVector2D(TopLeftUV.X + Dimension.X * 0.5f, TopLeftUV.Y + Dimension.Y);
    case ESpritePivotMode::Bottom_Right:
        return TopLeftUV + Dimension;

    default:
    case ESpritePivotMode::Custom:
        return CustomPivotPoint;
        break;
    };
}

void USimpleFlipbook::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    if (FramesPerSecond < 0.0f) {
        FramesPerSecond = 0.0f;
    }
    
    UObject::PostEditChangeProperty(PropertyChangedEvent);
}

void USimpleFlipbook::RebuildRenderData() {
    FSpriteGeometryCollection AlternateGeometry;

	switch (RenderGeometry.GeometryType)
	{
	case ESpritePolygonMode::Diced:
	case ESpritePolygonMode::SourceBoundingBox:
		CreatePolygonFromBoundingBox(RenderGeometry);
		break;

	case ESpritePolygonMode::TightBoundingBox:
		CreatePolygonFromBoundingBox(RenderGeometry);
		break;

	case ESpritePolygonMode::ShrinkWrapped:
		BuildGeometryFromContours(RenderGeometry);
		break;

	case ESpritePolygonMode::FullyCustom:
		// Do nothing special, the data is already in the polygon
		break;
	default:
		check(false); // unknown mode
	};

	// Determine the texture size
	UTexture2D* EffectiveTexture = GetBakedTexture();

	FVector2D TextureSize(1.0f, 1.0f);
	if (EffectiveTexture)
	{
		EffectiveTexture->ConditionalPostLoad();
		TextureSize = FVector2D(EffectiveTexture->GetImportedSize());
	}
	const float InverseWidth = 1.0f / TextureSize.X;
	const float InverseHeight = 1.0f / TextureSize.Y;

	// Adjust for the pivot and store in the baked geometry buffer
	const FVector2D DeltaUV(BakedSourceTexture != nullptr ? BakedSourceUV : FVector2D::ZeroVector);

	const float UnitsPerPixel = GetUnrealUnitsPerPixel();

	if ((RenderGeometry.GeometryType == ESpritePolygonMode::Diced) && (EffectiveTexture != nullptr))
	{
		const int32 AlphaThresholdInt = FMath::Clamp<int32>(RenderGeometry.AlphaThreshold * 255, 0, 255);
		FAlphaBitmap SourceBitmap(EffectiveTexture);
		SourceBitmap.ThresholdImageBothWays(AlphaThresholdInt, 255);

		bool bSeparateOpaqueSections = true;

		// Dice up the source geometry and sort into translucent and opaque sections
		RenderGeometry.Shapes.Empty();

        constexpr int32 X0 = 0;
        constexpr int32 Y0 = 0;
		const int32 X1 = GetSourceTexture()->GetSizeX();
		const int32 Y1 = GetSourceTexture()->GetSizeY();

		for (int32 Y = Y0; Y < Y1; Y += RenderGeometry.PixelsPerSubdivisionY)
		{
			const int32 TileHeight = FMath::Min(RenderGeometry.PixelsPerSubdivisionY, Y1 - Y);

			for (int32 X = X0; X < X1; X += RenderGeometry.PixelsPerSubdivisionX)
			{
				const int32 TileWidth = FMath::Min(RenderGeometry.PixelsPerSubdivisionX, X1 - X);

				if (!SourceBitmap.IsRegionEmpty(X, Y, X + TileWidth - 1, Y + TileHeight - 1))
				{
					FIntPoint Origin(X, Y);
					FIntPoint Dimension(TileWidth, TileHeight);
					
					SourceBitmap.TightenBounds(Origin, Dimension);

					bool bOpaqueSection = false;
					if (bSeparateOpaqueSections)
					{
						if (SourceBitmap.IsRegionEqual(Origin.X, Origin.Y, Origin.X + Dimension.X - 1, Origin.Y + Dimension.Y - 1, 255))
						{
							bOpaqueSection = true;
						}
					}

					const FVector2D BoxCenter = FVector2D(Origin) + (FVector2D(Dimension) * 0.5f);
					if (bOpaqueSection)
					{
						AlternateGeometry.AddRectangleShape(BoxCenter, Dimension);
					}
					else
					{
						RenderGeometry.AddRectangleShape(BoxCenter, Dimension);
					}
				}
			}
		}
	}

	// Triangulate the render geometry
	TArray<FVector2D> TriangluatedPoints;
	RenderGeometry.Triangulate(/*out*/ TriangluatedPoints, /*bIncludeBoxes=*/ true);

	// Triangulate the alternate render geometry, if present
	if (AlternateGeometry.Shapes.Num() > 0)
	{
		TArray<FVector2D> AlternateTriangluatedPoints;
		AlternateGeometry.Triangulate(/*out*/ AlternateTriangluatedPoints, /*bIncludeBoxes=*/ true);

		AlternateMaterialSplitIndex = TriangluatedPoints.Num();
		TriangluatedPoints.Append(AlternateTriangluatedPoints);
		RenderGeometry.Shapes.Append(AlternateGeometry.Shapes);
	}
	else
	{
		AlternateMaterialSplitIndex = INDEX_NONE;
	}

	// Bake the verts
	BakedRenderData.Empty(TriangluatedPoints.Num());
	for (int32 PointIndex = 0; PointIndex < TriangluatedPoints.Num(); ++PointIndex)
	{
		const FVector2D& SourcePos = TriangluatedPoints[PointIndex];

		const FVector2D PivotSpacePos = ConvertTextureSpaceToPivotSpace(SourcePos);
		const FVector2D UV(SourcePos + DeltaUV);

		new (BakedRenderData) FVector4(PivotSpacePos.X * UnitsPerPixel / Rows, PivotSpacePos.Y * UnitsPerPixel / Columns, UV.X * InverseWidth / Rows, UV.Y * InverseHeight / Columns);
	}

	check((BakedRenderData.Num() % 3) == 0);

	// Swap the generated vertices so they end up in counterclockwise order
	for (int32 SVT = 0; SVT < TriangluatedPoints.Num(); SVT += 3)
	{
		Swap(BakedRenderData[SVT + 2], BakedRenderData[SVT + 0]);
	}
}

FVector2D USimpleFlipbook::ConvertTextureSpaceToPivotSpace(FVector2D Input) const {
    const FVector2D Pivot = GetPivotPosition();

    const float X = Input.X - Pivot.X;
    const float Y = -Input.Y + Pivot.Y;

    return FVector2D(X, Y);
}

void USimpleFlipbook::CreatePolygonFromBoundingBox(FSpriteGeometryCollection& GeomOwner) const {
    FVector2D BoxSize = GetReferenceSprite()->GetSourceSize();
    FVector2D BoxPosition = FVector2D::Zero();

    // Recenter the box
    BoxPosition += BoxSize * 0.5f;

    // Put the bounding box into the geometry array
    GeomOwner.Shapes.Empty();
    GeomOwner.AddRectangleShape(BoxPosition, BoxSize);
}

void USimpleFlipbook::BuildGeometryFromContours(FSpriteGeometryCollection &GeomOwner) {
    // First trim the image to the tight fitting bounding box (the other pixels can't matter)
	FVector2D InitialBoxSizeFloat;
	FVector2D InitialBoxPosFloat;
	FindTextureBoundingBox(GeomOwner.AlphaThreshold, /*out*/ InitialBoxPosFloat, /*out*/ InitialBoxSizeFloat);

	const FIntPoint InitialPos((int32)InitialBoxPosFloat.X, (int32)InitialBoxPosFloat.Y);
	const FIntPoint InitialSize((int32)InitialBoxSizeFloat.X, (int32)InitialBoxSizeFloat.Y);

	// Find the contours
	TArray< TArray<FIntPoint> > Contours;

	// DK: FindContours only returns positive contours, i.e. outsides
	// Contour generation is simplified in FindContours by downscaling the detail prior to generating contour data
	FindContours(InitialPos, InitialSize, GeomOwner.AlphaThreshold, GeomOwner.DetailAmount, GetSourceTexture(), Contours);

	// Convert the contours into geometry
	GeomOwner.Shapes.Empty();
	for (int32 ContourIndex = 0; ContourIndex < Contours.Num(); ++ContourIndex)
	{
		TArray<FIntPoint>& Contour = Contours[ContourIndex];

		// Scale the simplification epsilon by the size we know the pixels will be
		int Divisor = GetDivisorFromDetail(InitialSize, GeomOwner.DetailAmount);
		SimplifyPoints(Contour, GeomOwner.SimplifyEpsilon * Divisor);

		if (Contour.Num() > 0)
		{
			FSpriteGeometryShape& NewShape = *new (GeomOwner.Shapes) FSpriteGeometryShape();
			NewShape.ShapeType = ESpriteShapeType::Polygon;
			NewShape.Vertices.Empty(Contour.Num());

			// Add the points
			for (int32 PointIndex = 0; PointIndex < Contour.Num(); ++PointIndex)
			{
				new (NewShape.Vertices) FVector2D(NewShape.ConvertTextureSpaceToShapeSpace(Contour[PointIndex]));
			}

			// Recenter them
			const FVector2D AverageCenterFloat = NewShape.GetPolygonCentroid();
			const FVector2D AverageCenterSnapped(FMath::RoundToInt(AverageCenterFloat.X), FMath::RoundToInt(AverageCenterFloat.Y));
			NewShape.SetNewPivot(AverageCenterSnapped);

			// Get intended winding
			NewShape.bNegativeWinding = !FGeomTools2D::IsPolygonWindingCCW(NewShape.Vertices);
		}
	}
}

void USimpleFlipbook::FindTextureBoundingBox(float AlphaThreshold, FVector2D &OutBoxPosition, FVector2D &OutBoxSize) {
    // Create an initial guess at the bounds based on the source rectangle
    int32 LeftBound = 0;
    int32 RightBound = GetSourceTexture()->GetSizeX() - 1;

    int32 TopBound = 0;
    int32 BottomBound = GetSourceTexture()->GetSizeY() - 1;

    const int32 AlphaThresholdInt = FMath::Clamp<int32>(AlphaThreshold * 255, 0, 255);
    FBitmap SourceBitmap(GetSourceTexture(), AlphaThresholdInt);
    if (SourceBitmap.IsValid())
    {
        // Make sure the initial bounds starts in the texture
        LeftBound = FMath::Clamp(LeftBound, 0, SourceBitmap.Width-1);
        RightBound = FMath::Clamp(RightBound, 0, SourceBitmap.Width-1);
        TopBound = FMath::Clamp(TopBound, 0, SourceBitmap.Height-1);
        BottomBound = FMath::Clamp(BottomBound, 0, SourceBitmap.Height-1);

        // Pull it in from the top
        while ((TopBound < BottomBound) && SourceBitmap.IsRowEmpty(LeftBound, RightBound, TopBound))
        {
            ++TopBound;
        }

        // Pull it in from the bottom
        while ((BottomBound > TopBound) && SourceBitmap.IsRowEmpty(LeftBound, RightBound, BottomBound))
        {
            --BottomBound;
        }

        // Pull it in from the left
        while ((LeftBound < RightBound) && SourceBitmap.IsColumnEmpty(LeftBound, TopBound, BottomBound))
        {
            ++LeftBound;
        }

        // Pull it in from the right
        while ((RightBound > LeftBound) && SourceBitmap.IsColumnEmpty(RightBound, TopBound, BottomBound))
        {
            --RightBound;
        }
    }

    OutBoxSize.X = RightBound - LeftBound + 1;
    OutBoxSize.Y = BottomBound - TopBound + 1;
    OutBoxPosition.X = LeftBound;
    OutBoxPosition.Y = TopBound;
}

void USimpleFlipbook::FindContours(const FIntPoint &ScanPos, const FIntPoint &ScanSize, float AlphaThreshold,
    float Detail, UTexture2D *Texture, TArray<TArray<FIntPoint>> &OutPoints) {
    OutPoints.Empty();

	if ((ScanSize.X <= 0) || (ScanSize.Y <= 0))
	{
		return;
	}

	// Neighborhood array (clockwise starting at -X,-Y; assuming prev is at -X)
	const int32 NeighborX[] = {-1, 0,+1,+1,+1, 0,-1,-1};
	const int32 NeighborY[] = {-1,-1,-1, 0,+1,+1,+1, 0};
	//                       0  1  2  3  4  5  6  7
	// 0 1 2
	// 7   3
	// 6 5 4
	const int32 StateMutation[] = {
		5, //from0
		6, //from1
		7, //from2
		0, //from3
		1, //from4
		2, //from5
		3, //from6
		4, //from7
	};

	int32 AlphaThresholdInt = FMath::Clamp<int32>(AlphaThreshold * 255, 0, 255);

	FBitmap FullSizeBitmap(Texture, AlphaThresholdInt);
	if (FullSizeBitmap.IsValid())
	{
		const int32 DownsampleAmount = GetDivisorFromDetail(ScanSize, Detail);

		FBitmap SourceBitmap((ScanSize.X + DownsampleAmount - 1) / DownsampleAmount, (ScanSize.Y + DownsampleAmount - 1) / DownsampleAmount, 0);
		for (int32 Y = 0; Y < ScanSize.Y; ++Y)
		{
			for (int32 X = 0; X < ScanSize.X; ++X)
			{
				SourceBitmap.SetPixel(X / DownsampleAmount, Y / DownsampleAmount, SourceBitmap.GetPixel(X / DownsampleAmount, Y / DownsampleAmount) | FullSizeBitmap.GetPixel(ScanPos.X + X, ScanPos.Y + Y));
			}
		}

		const int32 LeftBound = 0;
		const int32 RightBound = SourceBitmap.Width - 1;
		const int32 TopBound = 0;
		const int32 BottomBound = SourceBitmap.Height - 1;

		//checkSlow((LeftBound >= 0) && (TopBound >= 0) && (RightBound < SourceBitmap.Width) && (BottomBound < SourceBitmap.Height));

		// Create the 'output' boundary image
		FBoundaryImage BoundaryImage(FIntPoint(0, 0), FIntPoint(SourceBitmap.Width, SourceBitmap.Height));

		bool bInsideBoundary = false;

		for (int32 Y = TopBound - 1; Y < BottomBound + 2; ++Y)
		{
			for (int32 X = LeftBound - 1; X < RightBound + 2; ++X)
			{
				const bool bAlreadyTaggedAsBoundary = BoundaryImage.GetPixel(X, Y) > 0;
				const bool bPixelInsideBounds = (X >= LeftBound && X <= RightBound && Y >= TopBound && Y <= BottomBound);
				const bool bIsFilledPixel = bPixelInsideBounds && SourceBitmap.GetPixel(X, Y) != 0;

				if (bInsideBoundary)
				{
					if (!bIsFilledPixel)
					{
						// We're leaving the known boundary
						bInsideBoundary = false;
					}
				}
				else
				{
					if (bAlreadyTaggedAsBoundary)
					{
						// We're re-entering a known boundary
						bInsideBoundary = true;
					}
					else if (bIsFilledPixel)
					{
						// Create the output chain we'll build from the boundary image
						//TArray<FIntPoint>& Contour = *new (OutPoints) TArray<FIntPoint>();
						TArray<FIntPoint> Contour;

						// Moving into an undiscovered boundary
						BoundaryImage.SetPixel(X, Y, 1);
						new (Contour) FIntPoint(X, Y);

						// Current pixel
						int32 NeighborPhase = 0;
						int32 PX = X;
						int32 PY = Y;

						int32 EnteredStartingSquareCount = 0;
						int32 SinglePixelCounter = 0;

						for (;;)
						{
							// Test pixel (clockwise from the current pixel)
							const int32 CX = PX + NeighborX[NeighborPhase];
							const int32 CY = PY + NeighborY[NeighborPhase];
							const bool bTestPixelInsideBounds = (CX >= LeftBound && CX <= RightBound && CY >= TopBound && CY <= BottomBound);
							const bool bTestPixelPasses = bTestPixelInsideBounds && SourceBitmap.GetPixel(CX, CY) != 0;

							//UE_LOG(LogPaper2D, Log, TEXT("Outer P(%d,%d), C(%d,%d) Ph%d %s"),
							//	PX, PY, CX, CY, NeighborPhase, bTestPixelPasses ? TEXT("[BORDER]") : TEXT("[]"));

							if (bTestPixelPasses)
							{
								// Move to the next pixel

								// Check to see if we closed the loop
								if ((CX == X) && (CY == Y))
								{
// 									// If we went thru the boundary pixel more than two times, or
// 									// entered it from the same way we started, then we're done
// 									++EnteredStartingSquareCount;
// 									if ((EnteredStartingSquareCount > 2) || (NeighborPhase == 0))
// 									{
									//@TODO: Not good enough, will early out too soon some of the time!
										bInsideBoundary = true;
										break;
									}
// 								}

								BoundaryImage.SetPixel(CX, CY, NeighborPhase+1);
								new (Contour) FIntPoint(CX, CY);

								PX = CX;
								PY = CY;
								NeighborPhase = StateMutation[NeighborPhase];

								SinglePixelCounter = 0;
								//NeighborPhase = (NeighborPhase + 1) % 8;
							}
							else
							{
								NeighborPhase = (NeighborPhase + 1) % 8;

								++SinglePixelCounter;
								if (SinglePixelCounter > 8)
								{
									// Went all the way around the neighborhood; it's an island of a single pixel
									break;
								}
							}
						}
						
						// Trace the contour shape creating polygon edges
						TArray<FIntPoint>& ContourPoly = *new (OutPoints)TArray<FIntPoint>();
						TraceContour(/*out*/ContourPoly, Contour);

						// Remove collinear points from the result
						RemoveCollinearPoints(/*inout*/ ContourPoly);

						if (!FGeomTools2D::IsPolygonWindingCCW(ContourPoly))
						{
							// Remove newly added polygon, we don't support holes just yet
							OutPoints.RemoveAt(OutPoints.Num() - 1);
						}
						else
						{
							for (int ContourPolyIndex = 0; ContourPolyIndex < ContourPoly.Num(); ++ContourPolyIndex)
							{
								// Rescale and recenter contour poly
								FIntPoint RescaledPoint = ScanPos + ContourPoly[ContourPolyIndex] * DownsampleAmount;

								// Make sure rescaled point doesn't exceed the original max bounds
								RescaledPoint.X = FMath::Min(RescaledPoint.X, ScanPos.X + ScanSize.X);
								RescaledPoint.Y = FMath::Min(RescaledPoint.Y, ScanPos.Y + ScanSize.Y);

								ContourPoly[ContourPolyIndex] = RescaledPoint;
							}
						}
					}
				}
			}
		}
	}
}
#endif