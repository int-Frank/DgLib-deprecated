#!/usr/bin/python

import time
import sys
import os
import shutil
import subprocess

#--- Settings -----------------------------------------------------

#You will need to set the path to these
MSBuildPath = os.path.abspath("C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/Bin/MSBuild.exe")

#--- Functions ----------------------------------------------------

def Exit():
    print("Build Failed")
    sys.exit()


def ExecuteGenerator(a_cmd, a_workingDir = None):
    popen = subprocess.Popen(a_cmd, stdout=subprocess.PIPE, universal_newlines=True, cwd = a_workingDir)
    stdout_lines = iter(popen.stdout.readline, "")
    for stdout_line in stdout_lines:
        yield stdout_line

    popen.stdout.close()
    return_code = popen.wait()
    if return_code != 0:
        raise subprocess.CalledProcessError(return_code, cmd)

def Execute(a_cmd, a_workingDir = None):
    try:
        for line in ExecuteGenerator(a_cmd, a_workingDir):
            print(line)
        return True
    except OSError:
        print("OSError\n")
        return False
    except:
        return False
    
#--- Setup --------------------------------------------------------

pluginFileName = "ipc.dll"
pluginProjectName = "IPC_TCP_Plugin"
configurations = ["Debug", "Release"]
outputDirs = ["D:/dev/projects/DgLegendTools/output/DemoMapEditor/x64/", "D:/dev/projects/DgLegendTools/output/DemoStaticDataViewer/x64/"]

# These paths shouldn't need to change
#projectPath     = os.path.abspath("./src/samples/IPC_TCP_Plugin/IPC_TCP_Plugin.vcxproj")
projectPath     = os.path.abspath("./src/samples/Samples.sln")
#--- Main ---------------------------------------------------------

for configuration in configurations:
    
    #Build the main libs
    args = [MSBuildPath, projectPath, "/t:" + pluginProjectName,"/property:Configuration=" + configuration, "/property:Platform=x64", "/p:BuildProjectReferences=true"]
    if not Execute(args):
        print("Building failed! Exiting...")
        Exit()
    print("Build complete!")

    #Delete current plugins
    for outDir in outputDirs:
        outPath = outDir + configuration + "/"
        outFile = outPath + pluginFileName
        newFile = "./output/" + pluginProjectName + "/x64/" + configuration + "/" + pluginProjectName + ".dll"
        if (os.path.isdir(outPath)):
            if (os.path.isfile(outFile)):
                os.remove(outDir + configuration + "/" + pluginFileName)
            shutil.copy(newFile, outPath)
            os.rename(outPath + pluginProjectName + ".dll", outFile)

# Done!
print("Build Succeeded!")
