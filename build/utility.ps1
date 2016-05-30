#Set working directory to current path
$scriptPath = $MyInvocation.MyCommand.Path
$scriptDir = Split-Path $scriptPath
Push-Location $scriptDir

#Constants

$MSBuildPath = 'C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe'
$LibEXEPath = 'C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\amd64\lib.exe'
$DgLibFilePath = Resolve-Path '..\DgLib.sln'
$BuildFailedTitle = "Build Failed"
$DgLibLibName = "DgLib.lib"

#Functions

#type: Info, Warning, Error
function DisplayNotification ($type, $title, $message)
{
    [void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms")

    $objNotifyIcon = New-Object System.Windows.Forms.NotifyIcon 

    $objNotifyIcon.Icon = ".\resources\tray-icon.ico"

    $objNotifyIcon.BalloonTipIcon = $type
    $objNotifyIcon.BalloonTipText = $message 
    $objNotifyIcon.BalloonTipTitle = $title 
    $objNotifyIcon.Visible = $True 
    $objNotifyIcon.ShowBalloonTip(10000)
}