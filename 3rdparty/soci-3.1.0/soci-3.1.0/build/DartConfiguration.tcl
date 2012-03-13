# This file is configured by CMake automatically as DartConfiguration.tcl
# If you choose not to use CMake, this file may be hand configured, by
# filling in the required variables.


# Configuration directories and files
SourceDirectory: E:/ani/soci-3.1.0/soci-3.1.0
BuildDirectory: E:/ani/soci-3.1.0/soci-3.1.0/build

# Where to place the cost data store
CostDataFile: 

# Site is something like machine.domain, i.e. pragmatic.crd
Site: IBM-272E27711D6

# Build name is osname-revision-compiler, i.e. Linux-2.4.2-2smp-c++
BuildName: Win32-MSBuild

# Submission information
IsCDash: TRUE
CDashVersion: 
QueryCDashVersion: 
DropSite: my.cdash.org
DropLocation: /submit.php?project=SOCI
DropSiteUser: 
DropSitePassword: 
DropSiteMode: 
DropMethod: http
TriggerSite: 
ScpCommand: SCPCOMMAND-NOTFOUND

# Dashboard start time
NightlyStartTime: 00:00:00 EST

# Commands for the build/test/submit cycle
ConfigureCommand: "F:/Program Files/CMake 2.8/bin/cmake.exe" "E:/ani/soci-3.1.0/soci-3.1.0"
MakeCommand: C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe ALL_BUILD.vcxproj /p:Configuration=${CTEST_CONFIGURATION_TYPE}
DefaultCTestConfigurationType: Release

# CVS options
# Default is "-d -P -A"
CVSCommand: CVSCOMMAND-NOTFOUND
CVSUpdateOptions: -d -A -P

# Subversion options
SVNCommand: F:/Program Files/SlikSvn/bin/svn.exe
SVNUpdateOptions: 

# Git options
GITCommand: GITCOMMAND-NOTFOUND
GITUpdateOptions: 
GITUpdateCustom: 

# Generic update command
UpdateCommand: 
UpdateOptions: 
UpdateType: 

# Compiler info
Compiler: F:/Program Files/Microsoft Visual Studio 10.0/VC/bin/cl.exe

# Dynamic analysis and coverage
PurifyCommand: 
ValgrindCommand: 
ValgrindCommandOptions: 
MemoryCheckCommand: MEMORYCHECK_COMMAND-NOTFOUND
MemoryCheckCommandOptions: 
MemoryCheckSuppressionFile: 
CoverageCommand: COVERAGE_COMMAND-NOTFOUND

# Cluster commands
SlurmBatchCommand: SLURM_SBATCH_COMMAND-NOTFOUND
SlurmRunCommand: SLURM_SRUN_COMMAND-NOTFOUND

# Testing options
# TimeOut is the amount of time in seconds to wait for processes
# to complete during testing.  After TimeOut seconds, the
# process will be summarily terminated.
# Currently set to 25 minutes
TimeOut: 1500

UseLaunchers: 0
CurlOptions: 
# warning, if you add new options here that have to do with submit,
# you have to update cmCTestSubmitCommand.cxx

# For CTest submissions that timeout, these options
# specify behavior for retrying the submission
CTestSubmitRetryDelay: 5
CTestSubmitRetryCount: 3