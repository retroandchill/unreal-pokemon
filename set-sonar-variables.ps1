$env:SONAR_SCANNER_VERSION = "5.0.1.3006"
$env:SONAR_DIRECTORY = [System.IO.Path]::Combine($(get-location).Path,".sonar")
$env:SONAR_SCANNER_HOME = "$env:SONAR_DIRECTORY/sonar-scanner-$env:SONAR_SCANNER_VERSION-windows"