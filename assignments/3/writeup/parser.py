import csv
import sys
procs = 0
size =1 
total=2
sort=3
save=4
init=5
read=6
send=7
IO=8

l = []
counter=0

f = open('results.csv', 'rt')
try:
    reader = csv.reader(f)
    for row in reader:
        if counter > 0:
            l.append(row)
        counter+= 1;
finally:
    f.close()

# for row in l:
#     print row


def printGraph(s,t):
    for row in l:
        # print row[procs],row[size]
        if (int(row[size]) == s):
            print "(" +row[procs] +"," + row[t] + ")"

dic=[0,3,6,9, 12, 15, 18]

def printPrec(np):
    one = "6"
    two = "10"
    three = "14"
    print "(" +one + "," + str(float(l[np][sort])/float(l[np][total])*100) + ") " ,"(" +two + "," + str(float(l[np+1][sort])/float(l[np+1][total])*100) + ")"   ,"(" +three + "," + str(float(l[np+2][sort])/float(l[np+2][total])*100) + ")"
    print "(" +one + "," + str(float(l[np][save])/float(l[np][total])*100) + ") " ,"(" +two + "," + str(float(l[np+1][save])/float(l[np+1][total])*100) + ")"   ,"(" +three + "," + str(float(l[np+2][save])/float(l[np+2][total])*100) + ")"
    print "(" +one + "," + str(float(l[np][init])/float(l[np][total])*100) + ") " ,"(" +two + "," + str(float(l[np+1][init])/float(l[np+1][total])*100) + ")"   ,"(" +three + "," + str(float(l[np+2][init])/float(l[np+2][total])*100) + ")"
    print "(" +one + "," + str(float(l[np][IO]  )/float(l[np][total])*100) + ") " ,"(" +two + "," + str(float(l[np+1][IO]  )/float(l[np+1][total])*100) + ")"   ,"(" +three + "," + str(float(l[np+2][IO]  )/float(l[np+2][total])*100) + ")"
    print "(" +one + "," + str(float(l[np][send])/float(l[np][total])*100) + ") " ,"(" +two + "," + str(float(l[np+1][send])/float(l[np+1][total])*100) + ")"   ,"(" +three + "," + str(float(l[np+2][send])/float(l[np+2][total])*100) + ")"
    print "(" +one + "," + str(float(l[np][read])/float(l[np][total])*100) + ") " ,"(" +two + "," + str(float(l[np+1][read])/float(l[np+1][total])*100) + ")"   ,"(" +three + "," + str(float(l[np+2][read])/float(l[np+2][total])*100) + ")"

#print ""
#for k in dic:
#    printPrec(k)
#    print ""
print "init"
for k in [10000000, 100000000, 200000000]:
    print "\\addplot coordinates {"
    printGraph(k,init)
    print "}; \\addlegendentry{$10^7$}"
    print ""

# print "sort"
# for k in [10000000, 100000000, 200000000]:
#     print "\\addplot coordinates {"
#     printGraph(k,sort)
#     print "}; \\addlegendentry{$10^7$}"
#     print ""
#
# print ""
# print "IO"
# for k in [10000000, 100000000, 200000000]:
#     print "\\addplot coordinates {"
#     printGraph(k,IO)
#     print "}; \\addlegendentry{$10^7$}"
#
# print ""
# print "send"
# for k in [10000000, 100000000, 200000000]:
#     print "\\addplot coordinates {"
#    printGraph(k,send)
#    print "}; \\addlegendentry{$10^7$}"


