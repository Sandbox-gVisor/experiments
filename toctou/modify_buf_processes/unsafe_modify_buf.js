
function beforeWrite() {
    argv = accessors.getArgv()
    if (argv[0] === "./buf") {
        s = accessors.writeString(args.arg1, "HELLO WORLD")
    }
}

accessors.AddHookBefore(1, beforeWrite)
