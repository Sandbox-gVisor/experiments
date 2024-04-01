function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

accessors.print("3 hooks\n")
accessors.AddHookBefore(39, beforeGetPID) // getpid
accessors.AddHookAfter(39, beforeGetPID)

accessors.AddHookBefore(104, beforeGetPID) // getgid
