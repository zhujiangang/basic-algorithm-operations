@echo off

set DEFAULT_PASSWORD=password
set ADDITIONAL_PARAM=-v -storepass %DEFAULT_PASSWORD%
set ROOT_DN="CN=MyRootCA, OU=CA Test, O=MyRootCA Inc., L=LA, S=SAN, C=US"
set ALICE_DN="CN=Alice, OU=Alice OU, O=Alice Company, L=Paris, S=NY, C=FR"

REM clean all files firstly
if exist root.certs del /F /Q root.certs
if exist root.cer del /F /Q root.cer
if exist alice.certs del /F /Q alice.certs
if exist alice.cer del /F /Q alice.cer
if exist alice_csr.pem del /F /Q alice_csr.pem
if exist alice_signedby_root.cer del /F /Q alice_signedby_root.cer
if exist alice_new.cer del /F /Q alice_new.cer

if "%1"=="clean" goto END

keytool -genkeypair -alias root -dname %ROOT_DN% -validity 730 -keystore root.certs %ADDITIONAL_PARAM% -keypass %DEFAULT_PASSWORD%
keytool -list -keystore root.certs %ADDITIONAL_PARAM%

keytool -exportcert -alias root -file root.cer -keystore root.certs %ADDITIONAL_PARAM%
keytool -printcert -file root.cer -v

keytool -genkeypair -alias alice -dname %ALICE_DN% -validity 7300 -keystore alice.certs %ADDITIONAL_PARAM% -keypass %DEFAULT_PASSWORD%
keytool -list -keystore alice.certs %ADDITIONAL_PARAM%

keytool -exportcert -alias alice -file alice.cer -keystore alice.certs %ADDITIONAL_PARAM%
keytool -printcert -file alice.cer -v

REM issue the CSR by root.certs using java
call jsign.bat -keystore root.certs -alias root -infile alice.cer -outfile alice_signedby_root.cer -storepass %DEFAULT_PASSWORD% -keypass %DEFAULT_PASSWORD%
if not "%ERRORLEVEL%"=="0" ( 
  echo jsign.bat failed.
  goto END
)

REM import root CA firstly
keytool -importcert -keystore alice.certs -alias root -file root.cer %ADDITIONAL_PARAM% -noprompt

REM import signed cert
keytool -importcert -keystore alice.certs -alias alice -file alice_signedby_root.cer %ADDITIONAL_PARAM%
keytool -list -keystore alice.certs %ADDITIONAL_PARAM%
keytool -exportcert -alias alice -file alice_new.cer -keystore alice.certs %ADDITIONAL_PARAM%

:END
