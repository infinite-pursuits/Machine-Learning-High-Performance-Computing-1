import time
import numpy as np


def make_matrix(rows, cols):
    matrix = [[0.0 for i in range(cols)] for j in range(rows)]
    return matrix


def init(rows, cols, matrix):
    for i in range(rows):
        for j in range(cols):
            matrix[i][j] = 0.4+((i+j) % 40 - 20.0)/40.0
    return matrix


def flatten(rows, cols, matrix):
    flattened_mat = make_matrix(rows*cols, 1)
    k = 0
    for i in range(rows):
        for j in range(cols):
            flattened_mat[k][0] = matrix[i][j]
            k += 1
    return flattened_mat


def multiply(mat1, mat2):
    row2 = len(mat2)
    col2 = len(mat2[0])
    mult = make_matrix(1, col2)
    start = time.monotonic()
    for i in range(col2):
        for j in range(row2):
            mult[0][i] += mat1[j][0] * mat2[j][i]
    end = time.monotonic()
    elapsed_time = end-start
    return mult, elapsed_time


def relu(mat):
    row = len(mat)
    col = len(mat[0])
    start = time.monotonic()
    for i in range(row):
        for j in range(col):
            if mat[i][j] >= 0:
                mat[i][j] = mat[i][j]
            else:
                mat[i][j] = 0
    end = time.monotonic()
    elapsed_time = end-start
    return mat, elapsed_time


def c3():
    x0 = make_matrix(256, 256)
    x0 = init(256, 256, x0)
    w0 = make_matrix(65536, 4000)
    w0 = init(65536, 4000, w0)
    w1 = make_matrix(4000, 1000)
    w1 = init(4000, 1000, w1)
    x0 = flatten(256, 256, x0)
    intermediate, elapsed_time1 = multiply(x0, w0)
    z0, elapsed_time2 = relu(intermediate)
    z0 = flatten(1, len(z0[0]), z0)
    intermediate, elapsed_time3 = multiply(z0, w1)
    z1, elapsed_time4 = relu(intermediate)
    addition =sum(z1[0])
    return addition, elapsed_time1+elapsed_time2+elapsed_time3+elapsed_time4


def c4():
    x0 = np.empty((256, 256))
    x0 = init(256, 256, x0)

    w0 = np.empty((65536, 4000))
    w0 = init(65536, 4000, w0)

    w1 = np.empty((4000, 1000))
    w1 = init(4000, 1000, w1)
    x0 = x0.flatten()
    start = time.monotonic()
    z0 = np.matmul(x0, w0).clip(min=0.0)
    end = time.monotonic()
    elapsed_time1 = end - start
    z0 = z0.flatten()
    start = time.monotonic()
    z1 = np.matmul(z0, w1).clip(min=0.0)
    end = time.monotonic()
    elapsed_time2 = end - start
    addition = np.sum(z1)
    return addition, elapsed_time1+elapsed_time2


def main():
    addition, elapsed_time = c3()
    print("*C3*\nElapsed time: {} secs\nChecksum: {}".format(elapsed_time,addition))

    addition2, elapsed_time2 = c4()
    speedup = elapsed_time/elapsed_time2
    print("*C4*\nElapsed time: {} secs \nSpeedup: {} times\nChecksum: {}".format(elapsed_time2,speedup,addition2))


if __name__ == "__main__":
    main()
