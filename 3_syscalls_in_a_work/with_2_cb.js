function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

hooks.print("2 cb\n")
hooks.AddCbBefore(39, beforeGetPID) // getpid
hooks.AddCbAfter(39, beforeGetPID)
