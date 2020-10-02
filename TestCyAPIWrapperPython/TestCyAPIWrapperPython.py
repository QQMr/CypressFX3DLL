from ctypes import *
mylibc = cdll.LoadLibrary('CyAPIWrapperDLL.dll')

mylibc.fibonacci_init(1,1)
print("QQ")

mylibc.fibonacci_current.restype = c_ulonglong
mylibc.fibonacci_next.restype = c_bool;
while True:   #无限循环
    print( str(mylibc.fibonacci_index()) + ": " + str((mylibc.fibonacci_current())) + "\n" );
    #print( str((mylibc.fibonacci_next())) )
    if ( (mylibc.fibonacci_next()) == False ):
        break

QQ = 1


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