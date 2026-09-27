# Wrapper script to run NewDBMS with trace and SQL input from file
param(
    [string]$ExePath,
    [string]$SqlFile
)

# Read SQL file and output each line followed by newline to simulate interactive input
$content = Get-Content $SqlFile -Raw
$lines = $content -split "`n"
foreach ($line in $lines) {
    Write-Host $line
}

# Actually pipe the content through stdin
Get-Content $SqlFile | & $ExePath --trace
