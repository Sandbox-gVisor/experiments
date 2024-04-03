function beforeGetPID() {
    a = 0
    b = 1
    c = a + b
}

accessors.print("1 hook\n")
accessors.AddHookBefore(39, beforeGetPID) // getpid

