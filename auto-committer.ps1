$folders = Get-ChildItem ./Content/Graphics/Pokemon -Directory

foreach ($folder in $folders) {
    if ($folder -match "Icons") {
        continue
    }
    
    git add ./Content/Graphics/Pokemon/$folder/.
    git commit -m "Pokémon sprites for $folder"
}

foreach ($rev in $(git rev-list --reverse origin/feature/140-battle-animations..feature/140-battle-animations)) {
    git push origin ${rev}:feature/140-battle-animations
}