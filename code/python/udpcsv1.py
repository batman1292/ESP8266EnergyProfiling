import socket
import time
import csv
import struct

# A UDP server

# Set up a UDP server
UDPSock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
start_time = time.time()
datalist = []
ans = []
st_file = "testnewimu.csv"
# Listen on port 21567
# (to all IP addresses on this system)
listen_addr = ("",8081)
UDPSock.bind(listen_addr)
i = 0
k = 0
s = ''
# Report on all data packets received and
# where they came from in each case (as this is
# UDP, each may be from a different source and it's
# up to the server to sort this out!)
for j in range(6):
    ans.append(0)
while True:
        data,addr = UDPSock.recvfrom(1024)
        if(data == 'end'):
            # while(len(s.split(',')) < 301):
            #     s = s+"0,"
            if(len(s.split(',')) < 400):
                s = s+'0'
                datalist.append(s)
                with open(st_file, 'a') as csvwrite:
                    writer = csv.writer(csvwrite, delimiter='\n',dialect = 'excel-tab')
                    writer.writerow(datalist)
                    csvwrite.close()
                    datalist = []
            s=''
        else:
            i = 0
            # a = struct.unpack("B", data[0])[0] # b = unsigned
            # b = struct.unpack("B", data[1])[0] # B = signed
            # a = (a << 8)
            # ans[0] = a + b	
            # i = 2
            while(i < 12):
                # print(data)
                # struct.unpack("b", data[i])[0]
                a = struct.unpack("b", data[i])[0] # b = unsigned
                b = struct.unpack("B", data[i+1])[0] # B = signed
                a = (a << 8)
                ans[i/2] = a+b
                i=i+2;

            # ans[7] = struct.unpack("b", data[len(data)-1])[0]
            print(ans)
            # s=''
            for q in ans[0:len(ans)]:
                s = s+str(q)+','
            # s = s+str(ans[len(ans)-1])
            # datalist.append(s)
            #print datalist
        
            


        # if(i%1000==0):
        # 	print ("Time: %s"% (time.time()-start_time))
        # 	start_time = time.time()
        # i = i+1
