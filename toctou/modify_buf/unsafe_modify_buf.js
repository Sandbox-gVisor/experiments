
function beforeWrite() {
    argv = hooks.getArgv()
    if (argv[0] === "./buf") {
        s = hooks.writeString(args.arg1, "HELLO WORLD")
    }
}

hooks.AddCbBefore(1, beforeWrite)
