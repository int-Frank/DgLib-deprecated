#Set working directory to current path
$scriptPath = $MyInvocation.MyCommand.Path
$scriptDir = Split-Path $scriptPath
Push-Location $scriptDir

$ErrorActionPreference = "Stop"

.".\utility.ps1"


#Clean and create new deployment folder
Remove-Item -Recurse -Force "..\deploy\DgLib"

md "..\deploy\DgLib"
md "..\deploy\DgLib\3rd party"
md "..\deploy\DgLib\docs"
md "..\deploy\DgLib\include"
md "..\deploy\DgLib\lib"
md "..\deploy\DgLib\samples"

#Build DgLib Win32 & x64 solution

foreach ($platform in ("Win32", "x64"))
{
    Echo "Building ${platform}"
    $args = $DgLibFilePath, '/property:Configuration=Release', "/property:Platform=${platform}", '/t:Rebuild'
    $env:SUPPRESS_OUTPUT = 1
    & $MSBuildPath $args

    if ($LastExitCode -ne 0)
    {
        $ErrorMessage = "Failed to compile ${platform}. Stopping build process..."
        DisplayNotification "Error" $BuildFailedTitle $ErrorMessage
        Echo $ErrorMessage
        exit
    }

    Echo "Build Complete!"
    Echo "Running tests for platform ${platform}"

    $TestApp = Resolve-Path "..\output\Tests\${platform}\Release\Tests.exe"
    Start-Process $TestApp -NoNewWindow -Wait

    if ($LastExitCode -ne 0)
    {
        $ErrorMessage = "Tests failed ${platform}. Stopping build process..."
        DisplayNotification "Error" $BuildFailedTitle $ErrorMessage
        Echo $ErrorMessage
        exit
    }

    Echo "All tests passed!"
    Echo "Combining libraries"

    pushd "..\bin\${platform}\Release\"

    $args = "/OUT:${OutputLibName}", 'Engine.lib', 'Math.lib', 'Utility.lib'

    $LibEXEPath = $LibEXEPath32
    if ($platform -eq "x64")
    {
        $LibEXEPath = $LibEXEPath64
    }

    Start-Process $LibEXEPath $args -NoNewWindow -Wait

    if ($LastExitCode -ne 0)
    {
        $ErrorMessage = "Combining libs failed on ${platform}. Stopping build process..."
        DisplayNotification "Error" $BuildFailedTitle $ErrorMessage
        Echo $ErrorMessage
        exit
    }
    Echo "Combining libraries complete!"

    popd

    
    md "..\deploy\DgLib\lib\${platform}"
    copy "..\bin\${platform}\Release\DgLib.lib" "..\deploy\DgLib\lib\${platform}"
}

#Build Samples (to make sure they build)

#Copy samples to deployment folder

#Copy public headers
copy ..\src\core\public\*.* ..\deploy\DgLib\include

#Create docs
Echo "Creating docs"

#Need to run doxygen from the location of the doxy file
pushd .\doxygen
& $DoxygenPath "Doxyfile"

If ($FailOnBadDocs -and (Get-Content "doxygen-error-log.txt") -ne $Null) 
{
    $ErrorMessage = "Documentation contains errors. Stopping build process..."
    DisplayNotification "Error" $BuildFailedTitle $ErrorMessage
    Echo $ErrorMessage
    exit
}

popd

Echo "Finished creating docs!"

#Zip and copy archive to Google Drive

DisplayNotification "Info" "Build Success!" "Done"
Echo "Build Success!"