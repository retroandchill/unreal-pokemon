//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#include "Data/Command.h"

FName UCommand::GetID() const {
	return ID;
}

FText UCommand::GetText_Implementation() const {
	return Text;
}

bool UCommand::IsEnabled_Implementation() const {
	return true;
}

FSlateColor UCommand::GetColorAndOpacity_Implementation() const {
	return ColorAndOpacity;
}

FSlateFontInfo UCommand::GetFont_Implementation() const {
	return Font;
}

FText UCommand::GetOriginalText() const {
	return Text;
}

const FSlateColor& UCommand::GetOriginalColorAndOpacity() const {
	return ColorAndOpacity;
}

const FSlateFontInfo& UCommand::GetOriginalFont() const {
	return Font;
}
