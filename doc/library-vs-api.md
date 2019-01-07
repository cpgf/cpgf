<!--notoc-->
# Programming interface -- Library VS API

## Library VS API

The cpgf library exposes two kind of programming interfaces.  
One is called "library".  
The other is called "API".

Library is based on classes and functions.  
With this approach, the application binary interface (ABI) is not guaranteed to be compatible cross compilers and platforms.

API is basing on interfaces, which are in Windows Component Object Model (COM) style.  
With this approach, the application binary interface (ABI) is guaranteed to be compatible cross compilers and platforms, and also cross languages.
