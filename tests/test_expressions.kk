# number literals: 1 25
# grouping: (0) ( 2 - 3 )
# unary negation: -1
# arithmetic: + - * /
(0) + -1 + ( 2 - 3 ) * 4 + 25 / 5 # expect 0

# error
# (123          # Error at end: Expect ')' after expression.
# 123```        # Error: ```
# 1 2 3         # Error at '2': Expect end of expression.
