import argparse, random, sys

class File:
    def __init__(self, filename):
        if(filename):
            f = open(filename, 'r')
            self.lines = f.read().splitlines()
            f.close()
        else:
            self.lines = sys.stdin.read().splitlines()
            
        self.numlines = 0
        for line in self.lines:
            self.numlines+=1

    def chooseline(self):
        return random.choice(self.lines)
        
def main():
    if(args.FILE and args.input_range):
        print("can't have input range and file")
        return 1

    if(args.head_count and args.head_count<0):
        print("invalid count")
        return 1

    if(args.head_count == 0):
        return 0
    
    #if you enter an input range
    if(args.input_range):
        a = args.input_range.split('-')
        try:
            low = int(a[0])
            high = int(a[1])
        except ValueError:
            print("invalid range")
            return 1
        except IndexError:
            print("invalid range")
            return 1
        
        if(len(a)!=2 or high<low):
            print("invalid range")
            return 1
        list = []
        for i in range(low, high+1):
            list.append(i)
        random.shuffle(list)
        
        #check for repeats
        if(args.repeat and args.head_count):
            for i in range(0, args.head_count):
                print(random.randint(low, high))
        elif(args.repeat and not args.head_count):
            while(True):
                print(random.randint(low, high))

        elif(args.head_count):
            for i in range(0, args.head_count):
                if(i>=len(list)):
                   break
                print(list[i])
        else:
            for i in range(0, len(list)):
                print(list[i])
                
    #read in a file or stdin            
    else:    
        if(args.FILE and args.FILE!='-'):
            file = File(args.FILE)
        else:
            file = File("")

        #check repeat
        if(args.repeat and not args.head_count):
            while(True):
                l = file.chooseline()
                print(l)
        elif(args.repeat and args.head_count):
            for i in range(0, args.head_count):
                l = file.chooseline()
                print(l)
        else:
            inds = []
            for i in range(0, file.numlines):
                inds.append(i)
            random.shuffle(inds)
            
            if(args.head_count):
                for i in range(0, args.head_count):
                    if(i>=len(inds)):
                        break
                    print(file.lines[inds[i]])
            else:
                for i in range(0, file.numlines):
                    print(file.lines[inds[i]])
                    
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    # parser.add_argument(...)
    parser.add_argument("FILE", nargs='?')
    parser.add_argument("-i", "--input-range")
    parser.add_argument("-n", "--head-count", type=int)
    parser.add_argument("-r", "--repeat", action='store_true')
    args = parser.parse_args()
    main()
    # call other functions that use args.some_option etc.
    # this main function should be as minimal as possible
