function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

accessors.AddHookBefore(39, beforeGetPID) // getpid
