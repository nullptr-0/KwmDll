# KwmDll
Kwm Encryption & Decryption Dynamic Link Library

# How to use
```
Other operations
1: Place the KwmDll.lib in yout project directory
2: Add `#include "KwmDll.hpp"` to your source
Other operations
```

# Functions
Name | Usage | Params
------------ | ------------- | -------------
kwmEncS | Encrypt, Single(No seperated key file) | Param (const char* fn): Name of file to encrypt
kwmDecS | Decrypt, Single(No seperated key file) | Param (const char* fn): Name of file to decrypt
kwmEncD | Encrypt, Dual(With seperated key file) | Param1 (const char* fn): see kwmEncS, Param2 (const char* pswFn): Name of file to store the key
kwmDecD | Decrypt, Dual(With seperated key file) | Param1 (const char* fn): see kwmDecS, Param2 (const char* pswFn): Name of file that stores the key
