function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

hooks.AddCbBefore(39, beforeGetPID) // getpid
hooks.AddCbAfter(39, beforeGetPID)

hooks.AddCbBefore(110, beforeGetPID) // getpgid
