
find $1 -type f -exec sed -i 's/B_B_B_BOOL /boolean /g' {} \;
find $1 -type f -exec sed -i 's/B_B_B_TRUE /true /g' {} \;
find $1 -type f -exec sed -i 's/B_B_B_FALSE /false /g' {} \;

find $1 -type f -exec sed -i 's/B_B_BOOL /boolean /g' {} \;
find $1 -type f -exec sed -i 's/B_B_TRUE /true /g' {} \;
find $1 -type f -exec sed -i 's/B_B_FALSE /false /g' {} \;

find $1 -type f -exec sed -i 's/B_BOOL /boolean /g' {} \;
find $1 -type f -exec sed -i 's/B_TRUE /true /g' {} \;
find $1 -type f -exec sed -i 's/B_FALSE /false /g' {} \;


find $1 -type f -exec sed -i 's/BOOL /boolean /g' {} \;
find $1 -type f -exec sed -i 's/TRUE /true /g' {} \;
find $1 -type f -exec sed -i 's/FALSE /false /g' {} \;
find $1 -type f -exec sed -i 's/TRUE;/true; /g' {} \;
find $1 -type f -exec sed -i 's/FALSE;/false; /g' {} \;

find $1 -type f -exec sed -i 's/TRUE)/true) /g' {} \;
find $1 -type f -exec sed -i 's/FALSE)/false) /g' {} \;

find $1 -type f -exec sed -i 's/QWORD /uint64_t /g' {} \;
find $1 -type f -exec sed -i 's/DWORD /uint32_t /g' {} \;
find $1 -type f -exec sed -i 's/WORD /uint16_t /g' {} \;

find $1 -type f -exec sed -i 's/BYTE /uint8_t /g' {} \;
find $1 -type f -exec sed -i 's/UINT /uint32_t /g' {} \;
find $1 -type f -exec sed -i 's/UINT8 /uint8_t /g' {} \;
find $1 -type f -exec sed -i 's/UINT16 /uint16_t /g' {} \;
find $1 -type f -exec sed -i 's/UINT32 /uint32_t /g' {} \;
find $1 -type f -exec sed -i 's/UINT64 /uint64_t /g' {} \;

find $1 -type f -exec sed -i 's/INT8 /int8_t /g' {} \;
find $1 -type f -exec sed -i 's/INT16 /int16_t /g' {} \;
find $1 -type f -exec sed -i 's/INT32 /int32_t /g' {} \;
find $1 -type f -exec sed -i 's/INT64 /int64_t /g' {} \;
