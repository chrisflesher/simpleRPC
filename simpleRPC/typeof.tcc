#ifndef __SIMPLE_RPC_TYPEOF_TCC__
#define __SIMPLE_RPC_TYPEOF_TCC__


/*
 * Type encoding functions.
 *
 * For more information about the encoding:
 * https://docs.python.org/2/library/struct.html#format-characters
 */

String _typeof(char) {
  return "c";
}

String _typeof(char *) {
  return "s";
}

String _typeof(const char *) {
  return "s";
}

String _typeof(signed char) {
  return "b";
}

String _typeof(unsigned char) {
  return "B";
}

String _typeof(short int) {
  return "<h";
}

String _typeof(unsigned short int) {
  return "<H";
}

String _typeof(long int) {
  return "<l";
}

String _typeof(unsigned long int) {
  return "<L";
}

String _typeof(long long int) {
  return "<q";
}

String _typeof(unsigned long long int) {
  return "<Q";
}

String _typeof(float) {
  return "<f";
}

/*
 * The {int} and {double} type sizes vary between boards, see:
 * https://www.arduino.cc/reference/en/language/variables/data-types/
 */
String _typeof(int) {
  if (sizeof(int) == 2) {
    return "<h";
  }
  return "<i";
}

String _typeof(unsigned int) {
  if (sizeof(int) == 2) {
    return "<H";
  }
  return "<I";
}

String _typeof(double) {
  if (sizeof(int) == 4) {
    return "<f";
  }
  return "<d";
}

/**
 * Recursion terminator for {_writeParameterTypes}.
 */
String _writeParameterTypes(void (*)(void)) {
  return "";
}

/**
 * Write the types of all function parameters to serial.
 *
 * We isolate the first parameter type {T} from function pointer {*f_}. This
 * type is used to instantiate the variable {data}, which is passed to
 * {_typeof()} to encode its type. The first parameter type {T} is removed from
 * function pointer {*f_} in the recursive call.
 *
 * @arg {void (*)(T, Args...)} f_ - Dummy function pointer.
 */
template<class T, class... Args>
String _writeParameterTypes(void (*f_)(T, Args...)) {
  T data;

  return " " + _typeof(data) + _writeParameterTypes((void (*)(Args...))f_);
}


/**
 * Describe the signature of a function that does not return a value.
 *
 * @arg {void (*)(Args...)} f - Function pointer.
 */
template<class... Args>
String describeSignature(void (*f)(Args...)) {
  return ":" + _writeParameterTypes(f);
}

/**
 * Describe the signature of a function that does return a value.
 *
 * We prepare a dummy function pointer, referred to as {f_} in the template
 * functions above, which will be used to isolate parameter types. The return
 * type of this function pointer is removed to avoid unneeded template
 * expansion.
 *
 * @arg {T (*)(Args...)} f - Function pointer.
 */
template<class T, class... Args>
String describeSignature(T (*f)(Args...)) {
  T data;

  return _typeof(data) + ":" + _writeParameterTypes((void (*)(Args...))f);
}

#endif