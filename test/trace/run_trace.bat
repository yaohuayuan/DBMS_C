@echo off
cd /d %1
type trace_smoke_input.sql | bin\NewDBMS.exe --trace
