@echo off

if exist demoCA rmdir /S /Q demoCA
if exist exts del /F /Q exts
if exist .rnd del /F /Q .rnd
if exist cacert.pem del /F /Q cacert.pem
if exist caprivkey.pem del /F /Q caprivkey.pem
if exist cacert_pem.pem del /F /Q cacert_pem.pem
if exist alice_signedby_root.pem del /F /Q alice_signedby_root.pem
if exist alice_signedby_root_pem.pem del /F /Q alice_signedby_root_pem.pem

if exist alice.certs del /F /Q alice.certs
if exist alice.cer del /F /Q alice.cer
if exist alice_csr.pem del /F /Q alice_csr.pem

if "%1"=="clean" goto END

mkdir demoCA\newcerts
touch demoCA\index.txt
echo 8899> demoCA/serial

touch exts
echo [x509_extensions]>>exts
echo basicConstraints=CA:true>>exts

set DEFAULT_PASSWORD=password
set ADDITIONAL_PARAM=-v -storepass %DEFAULT_PASSWORD%
set ALICE_DN="CN=Alice, OU=Alice OU, O=Alice Company, L=Paris, S=NY, C=FR"

REM Create the CA's keypair and self-signed certificate
openssl req -verbose -x509 -new -nodes -set_serial 1234 -subj "/CN=MyRootCA/OU=CA Test/O=MyRootCA Inc./L=LA/ST=SAN/C=US" -days 7300 -out cacert.pem -keyout caprivkey.pem

REM View The Contents Of A Certificate
echo View The Contents Of the Certificate: cacert.pem
openssl x509 -text -noout -in cacert.pem

echo View The modulus Of the private key: caprivkey.pem
openssl rsa -in caprivkey.pem -noout -modulus

REM generate alice certificate
keytool -genkeypair -alias alice -dname %ALICE_DN% -validity 7300 -keystore alice.certs %ADDITIONAL_PARAM% -keypass %DEFAULT_PASSWORD%
keytool -list -keystore alice.certs %ADDITIONAL_PARAM%

keytool -exportcert -alias alice -file alice.cer -keystore alice.certs %ADDITIONAL_PARAM%
keytool -printcert -file alice.cer -v

REM generate CSR by alice
keytool -certreq -keystore alice.certs -alias alice -file alice_csr.pem %ADDITIONAL_PARAM%

REM sign alice csr by cacert -batch 
openssl ca -batch -days 365 -cert cacert.pem -keyfile caprivkey.pem -in alice_csr.pem -out alice_signedby_root.pem

REM View The Contents Of A Certificate
echo View The Contents Of the Certificate: alice_signedby_root.pem
openssl x509 -text -noout -in alice_signedby_root.pem


REM convert CA cert format to PEM format
openssl x509 -in cacert.pem -out cacert_pem.pem -outform PEM
openssl x509 -in alice_signedby_root.pem -out alice_signedby_root_pem.pem -outform PEM

keytool -importcert -alias root -file cacert_pem.pem -keystore alice.certs %ADDITIONAL_PARAM% -noprompt
keytool -list -keystore alice.certs %ADDITIONAL_PARAM%
keytool -importcert -alias alice -file alice_signedby_root_pem.pem -keystore alice.certs %ADDITIONAL_PARAM% -noprompt
keytool -list -keystore alice.certs %ADDITIONAL_PARAM%

:END
