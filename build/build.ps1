#Set working directory to current path
$scriptPath = $MyInvocation.MyCommand.Path
$scriptDir = Split-Path $scriptPath
Push-Location $scriptDir

#Get time
$date = Get-Date

#Constants
$LogFileName = "log-" + $date.Year + '-' + $date.Month + '-' + $date.Day + '__' + $date.Hour + '-' + $date.Minute + '-' + $date.Second + '.txt'

#Set up log file
if(!(Test-Path -Path '.\logs' ))
{
    New-Item -ItemType directory -Path '.\logs'
}

New-Item ".\logs\${LogFileName}" -type file

$LogFile = Resolve-Path ".\logs\${LogFileName}"

iex ".\work.ps1" | Tee-Object -file $LogFile