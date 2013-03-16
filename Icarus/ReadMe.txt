Uregent:
  PreProcessor
  Mutli-Generational Garbage Collection
  Parameter space on stack (because of size problems)
  Operators

  Sort out deallocation of used data-types.

  Default preprocessor definitions (debug/release/x86/x64/etc)

  Base Object class
  Class Constructors (with nullification code postfix)










// TRUNCATING OVERWRITING SIGN BITS?
//   Use SHL/SHR instead of AND?
//   Use of CWD, CBW, CDQ?

// Assign to local or parameter -> Different storage sizes (parameter is promoted to int (depromote?), variable is not) ???

// Should use correct registers for operations? Otherwise they may not take sign bit into consideration?

// Add class fields/methods for all the default types (eg. int.maxValue, int.Parse)


// Check null access violations
// Call debug functions

// invoke executegc

// unhandledExceptionFilter

preprocessor

double

inheretence
object class
move parameters to local space on the stack (some parameters are promoted to int, there for parameter size != storage size).


- Parameters offsets may also not be correct (overwritting the wrong data if multiple promoted-to-int variables are stored together)

byte byte byte
[1][1][1] <- Should be stored like that, offsets assume that.

[4][4][4] <- Promoted to int so stored like this.

If we try and access the secodn byte parameter, we would end up ovewritting the second byte of the first parameter!

class casting
class default value of fields
static indirection StringHelper::Blah()
Constructor
Destructor

arrays

TODO:

  Fix memory leak in GC.
  Override malloc to check for mem leaks and track allocation.
  
  Overloadable Operators
  Native string class

  Operators for all classes
  Casting to/from strings

  Float / Double

  Remove LogicalAnd/LogicalOr from data type and implement
  in reusable code.

  Truncate bytes and such when converted from strings
