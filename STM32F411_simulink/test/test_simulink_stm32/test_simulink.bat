set MATLAB=D:\Program Files\Polyspace\R2020a

call  "\\LAPTOP-F8T9AKQ0\D$\Program Files\Polyspace\R2020a\bin\win64\checkMATLABRootForDriveMap.exe" "\\LAPTOP-F8T9AKQ0\D$\Program Files\Polyspace\R2020a"  > mlEnv.txt
for /f %%a in (mlEnv.txt) do set "%%a"\n
"%MATLAB%\bin\%MATLAB_WIN_VER%\gmake" -f test_simulink.mk MATLAB_ROOT=%MATLAB_ROOT% ALT_MATLAB_ROOT=%ALT_MATLAB_ROOT% MATLAB_BIN=%MATLAB_BIN% ALT_MATLAB_BIN=%ALT_MATLAB_BIN%  GENERATE_ASAP2=0 ISPROTECTINGMODEL=NOTPROTECTING OPTS="-DTID01EQ=0"
