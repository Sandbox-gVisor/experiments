function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

hooks.print("1 cb\n")
hooks.AddCbBefore(39, beforeGetPID) // getpid

