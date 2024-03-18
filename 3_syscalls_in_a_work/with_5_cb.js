function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

hooks.print("5 cb\n")
hooks.AddCbBefore(39, beforeGetPID) // getpid
hooks.AddCbAfter(39, beforeGetPID)

hooks.AddCbBefore(110, beforeGetPID) // getpgid
hooks.AddCbAfter(110, beforeGetPID)

hooks.AddCbBefore(102, beforeGetPID) // getuid
