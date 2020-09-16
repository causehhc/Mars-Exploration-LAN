import time
import keyboard
import socket
import threading
from queue import Queue

client_list = []       # 客户端列表


def get_key():
    ans = [0, 0]
    sta = 0

    key_w = keyboard.is_pressed('w')
    key_s = keyboard.is_pressed('s')
    key_a = keyboard.is_pressed('a')
    key_d = keyboard.is_pressed('d')
    key_q = keyboard.is_pressed('q')
    key_e = keyboard.is_pressed('e')
    key_c = keyboard.is_pressed('c')

    if key_e and not key_q:
        ans[0] = 1
    if key_q and not key_e:
        ans[0] = 2

    if key_w and not key_s and not key_a and not key_d:
        ans[1] = 1
    if key_w and not key_s and not key_a and key_d:
        ans[1] = 2
    if not key_w and not key_s and not key_a and key_d:
        ans[1] = 3
    if not key_w and key_s and not key_a and key_d:
        ans[1] = 4
    if not key_w and key_s and not key_a and not key_d:
        ans[1] = 5
    if not key_w and key_s and key_a and not key_d:
        ans[1] = 6
    if not key_w and not key_s and key_a and not key_d:
        ans[1] = 7
    if key_w and not key_s and key_a and not key_d:
        ans[1] = 8

    if key_c:
        sta = 1

    return ans, sta


def server_target(sever_socket, out_q):
    while True:
        content = sever_socket.recv(2048).decode('UTF-8')    # 接受客户端发送的数据
        if content is not None:
            out_q.put(content.encode('UTF-8'))
            print("Receive: " + content)            # 打印客户端发送的数据


def control(in_q):
    while True:
        print("waiting for word")
        flag = input('')
        if flag == "ok":
            while True:
                line, sta = get_key()
                if sta:
                    break
                str_line = str(line)
                str_line = str_line.replace(', ', '')
                for cl in client_list:  # 将数据送回给每个客户端
                    byte_line0 = str_line[0].encode('UTF-8')
                    byte_line1 = str_line[1].encode('UTF-8')
                    byte_line2 = str_line[2].encode('UTF-8')
                    byte_line3 = str_line[3].encode('UTF-8')
                    cl.send(byte_line0)
                    print("send: " + str_line[0], end=' ')
                    while True:
                        data = in_q.get()
                        if data == byte_line0:
                            break
                    cl.send(byte_line1)
                    print("send: " + str_line[1], end=' ')
                    while True:
                        data = in_q.get()
                        if data == byte_line1:
                            break
                    cl.send(byte_line2)
                    print("send: " + str_line[2], end=' ')
                    while True:
                        data = in_q.get()
                        if data == byte_line2:
                            break
                    cl.send(byte_line3)
                    print("send: " + str_line[3], end=' ')
                    while True:
                        data = in_q.get()
                        if data == byte_line3:
                            break


if __name__ == '__main__':
    q = Queue()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    s.bind(('192.168.1.225', 8080))
    s.listen()
    while True:
        c, addr = s.accept()
        print(addr)
        client_list.append(c)  # 将所有客户端对应的socket保存在列表中
        threading.Thread(target=server_target, args=(c, q,)).start() # 为客户端对应的socket启动对应的线程
        threading.Thread(target=control, args=(q, )).start()
