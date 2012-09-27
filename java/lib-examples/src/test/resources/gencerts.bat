@echo off

set DEFAULT_PASSWORD=password
set ADDITIONAL_PARAM=-v -storepass %DEFAULT_PASSWORD% -keypass %DEFAULT_PASSWORD%
set ROOT_DN="CN=Root Name, OU=CA Team, O=CA Inc., L=LA, S=SAN C=US"
set ALICE_DN="CN=Alice, OU=Alice OU, O=Some Company, L=LA, S=SAN C=US"

if exist root.certs del /F /Q root.certs
keytool -genkeypair -alias root -dname %ROOT_DN% -validity 730 -keystore root.certs %ADDITIONAL_PARAM%
keytool -list -v -keystore root.certs -storepass %DEFAULT_PASSWORD%

if exist root.cer del /F /Q root.cer
keytool -exportcert -alias root -file root.cer -keystore root.certs %ADDITIONAL_PARAM%
keytool -printcert -v -file root.cer

if exist alice.certs del /F /Q alice.certs
keytool -genkeypair -alias alice -dname %ALICE_DN% -validity 730 -keystore alice.certs %ADDITIONAL_PARAM%
keytool -list -v -keystore alice.certs -storepass %DEFAULT_PASSWORD%

if exist alice.cer del /F /Q alice.cer
keytool -exportcert -alias alice -file alice.cer -keystore alice.certs %ADDITIONAL_PARAM%
keytool -printcert -v -file alice.cer
