from ctypes import *
mylibc = cdll.LoadLibrary('CyAPIWrapperDLL.dll')

mylibc.fibonacci_init(1,1)
print("QQ")

mylibc.fibonacci_current.restype = c_ulonglong
mylibc.fibonacci_next.restype = c_bool;
mylibc.GetReceiveData.argtypes = [POINTER(c_uint32) , POINTER(c_int) ]
while False:   #无限循环
    print( str(mylibc.fibonacci_index()) + ": " + str((mylibc.fibonacci_current())) + "\n" );
    #print( str((mylibc.fibonacci_next())) )
    if ( (mylibc.fibonacci_next()) == False ):
        break

QQ = 1

x = (c_ulong*5)();

LENG = (c_int*1)();
cast(x, POINTER(c_ulong))
cast(LENG, POINTER(c_int))

y = c_int;
mylibc.GetReceiveData(x,byref(y));


  #// Write out the sequence values until overflow.
  #  do {
  #      std::cout << fibonacci_index() << ": "
  #          << fibonacci_current() << std::endl;
  #  } while (fibonacci_next());
  #  // Report count of values written before overflow.
  #  std::cout << fibonacci_index() + 1 <<
  #      " Fibonacci sequence values fit in an " <<
  #      "unsigned 64-bit integer." << std::endl;


print("QQ2")