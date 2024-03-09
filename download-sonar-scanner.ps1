./set-sonar-variables.ps1
if (-not (Test-Path $env:SONAR_SCANNER_HOME)) {
    rm $env:SONAR_SCANNER_HOME -Force -Recurse -ErrorAction SilentlyContinue
    New-Item -path $env:SONAR_SCANNER_HOME -type directory
    (New-Object System.Net.WebClient).DownloadFile("https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$env:SONAR_SCANNER_VERSION-windows.zip", "$env:SONAR_DIRECTORY/sonar-scanner.zip")
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    [System.IO.Compression.ZipFile]::ExtractToDirectory("$env:SONAR_DIRECTORY/sonar-scanner.zip", "$env:SONAR_DIRECTORY")
    rm ./.sonar/sonar-scanner.zip -Force -ErrorAction SilentlyContinue
    $env:SONAR_SCANNER_OPTS = "-server"

    rm "$env:SONAR_DIRECTORY/build-wrapper-win-x86" -Force -Recurse -ErrorAction SilentlyContinue
    (New-Object System.Net.WebClient).DownloadFile("https://sonarcloud.io/static/cpp/build-wrapper-win-x86.zip", "$env:SONAR_DIRECTORY/build-wrapper-win-x86.zip")
    [System.IO.Compression.ZipFile]::ExtractToDirectory("$env:SONAR_DIRECTORY/build-wrapper-win-x86.zip", "$env:SONAR_DIRECTORY")
}