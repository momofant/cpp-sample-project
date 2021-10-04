
def factorial(N):
  F = 1
  for i in range(1, N+1):
    F = F * i
  return F


print(factorial(100))