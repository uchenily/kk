"hello " + "world" + "!" # expect "hello world!"
# "string" == "string" # expect true
# "aaaaa" != "bbbbbbb" # expect true
# "bbbbbbbb" != "aaaa" # expect true

# error
# "hello " - "world" # expect runtimeError("Operands must be two numbers.")
# "hello " + 123 # expect runtimeError("Operands must be two strings.")
# 123 + " hello" # expect runtimeError("Operands must be two numbers.")
