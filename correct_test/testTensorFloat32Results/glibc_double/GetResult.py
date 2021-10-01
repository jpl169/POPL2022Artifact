#  , "SinpiLogFile.txt", "CospiLogFile.txt"


files = ["LogLogFile.txt", "Log2LogFile.txt", "Log10LogFile.txt", "ExpLogFile.txt", "Exp2LogFile.txt", "Exp10LogFile.txt", "SinhLogFile.txt", "CoshLogFile.txt"]

for filename in files:
    with open(filename) as f:
        lines = f.readlines()
        lines = [l.split() for l in lines]
        numbers = [l[-1] for l in lines]
        for n in numbers :
            print(n + ",", end = '')
        print()
