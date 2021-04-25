#!/usr/bin/env bash


## usage: Example - ./uploadKalturaPlayerYouiFramworks.sh <fileName> <path in S3 prebuilt>
## ./uploadKalturaPlayerYouiFramworks.sh KalturaPlayerYouiFrameworks-v1.0.1.zip kux/ios/v1.0.1

## download Example:
## curl -L -o KalturaPlayerYouiFrameworks-v1.0.1.zip "https://playkit.s3.amazonaws.com/prebuilt/kux/ios/v1.0.1/KalturaPlayerYouiFrameworks-v1.0.1.zip"  

set -e

if [[ -z "${AWS_ACCESS_KEY_ID}" ]]; then
   echo "ERROR AWS_ACCESS_KEY_ID env is undefined"
   exit 1;
fi

if [[ -z "${AWS_SECRET_ACCESS_KEY}" ]]; then
   echo "ERROR AWS_SECRET_ACCESS_KEY env is undefined"
   exit 1;
fi



AWS_S3_BUCKET=playkit
AWS_S3_PREFIX=prebuilt


file="$1"
path="$2"

if [[ -z "${file}" ]]; then
   echo "ERROR first argument <file name> is undefined"
   exit 1;
fi

if [[ -z "${path}" ]]; then
   echo "ERROR second argument <path> is undefined"
   exit 1;
fi

basename=$(basename "$file")
bucket="$AWS_S3_BUCKET"
prefix="$AWS_S3_PREFIX/$path"
resource="/${bucket}/${prefix}/${basename}"
contentType="application/octet-stream"
dateValue=$(date -R)
stringToSign="PUT\n\n${contentType}\n${dateValue}\n${resource}"
s3Key="${AWS_ACCESS_KEY_ID}"
s3Secret="${AWS_SECRET_ACCESS_KEY}"
signature=$(echo -en "$stringToSign" | openssl sha1 -hmac "$s3Secret" -binary | base64)
curl -X PUT -T "${file}" \
  -H "Host: ${bucket}.s3.amazonaws.com" \
  -H "Date: ${dateValue}" \
  -H "Content-Type: ${contentType}" \
  -H "Authorization: AWS ${s3Key}:${signature}" \
  "https://${bucket}.s3.amazonaws.com/${prefix}/${basename}"


