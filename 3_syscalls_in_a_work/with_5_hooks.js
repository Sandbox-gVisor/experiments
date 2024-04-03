function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

accessors.print("5 hooks\n")
accessors.AddHookBefore(39, beforeGetPID) // getpid
accessors.AddHookAfter(39, beforeGetPID)

accessors.AddHookBefore(104, beforeGetPID) // getgid
accessors.AddHookAfter(104, beforeGetPID)

accessors.AddHookBefore(102, beforeGetPID) // getuid
