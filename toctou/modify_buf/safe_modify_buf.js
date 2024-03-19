
function beforeWrite() {
    argv = hooks.getArgv()
    if (argv[0] === "./buf") {
        hooks.stopThreads()
        s = hooks.writeString(args.arg1, "HELLO WORLD")
    }
}

function afterWrite() {
    if (argv[0] === "./buf") {
        hooks.resumeThreads()
    }
}

hooks.AddCbBefore(1, beforeWrite)
hooks.AddCbAfter(1, afterWrite)
