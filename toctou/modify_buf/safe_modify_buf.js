
function beforeWrite() {
    argv = accessors.getArgv()
    if (argv[0] === "./buf") {
        accessors.stopThreads()
        s = accessors.writeString(args.arg1, "HELLO WORLD")
    }
}

function afterWrite() {
    if (argv[0] === "./buf") {
        accessors.resumeThreads()
    }
}

accessors.AddHookBefore(1, beforeWrite)
accessors.AddHookAfter(1, afterWrite)
