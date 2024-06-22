from PIL import Image
import math
import numpy as np

# convolution 연산은 cross-correlation 연산과 다르게 filter의 역방향 원소와 
# #이미지의 정방향 원소간의 원소곱을 합연산하여 계산함(-3)


def gauss1d(sigma):
    n = int(np.ceil(6 * sigma))  # 필터 길이 계산
    if n % 2 == 0:  # 홀수로 만들기
        n += 1
    center = n // 2  # 중심 인덱스 계산
    x = np.arange(0, n) - center  # 중심을 기준으로 좌표 설정
    gaus1d_filter = np.exp(-x**2 / (2 * sigma**2))  # 가우스 함수 계산 (문제 제시 kernel)
    gaus1d_filter /= gaus1d_filter.sum()  # 정규화
    return gaus1d_filter

def gauss2d(sigma):
    # 1차원 가우스 필터를 생성
    g1d = gauss1d(sigma)
    # 2D 가우스 필터 생성
    gauss2d_filter = np.outer(g1d, np.transpose(g1d))
    #gauss2d_filter = np.outer(g1d, g1d)
    return gauss2d_filter

def convolve2d(array, filter):
    filter_width, filter_height = filter.shape
    filter = np.flip(np.flip(filter, 0), 1)
    
    padded_width = (filter_width-1) // 2
    padded_height = (filter_height-1) // 2

    padded_array = np.pad(array, ((padded_height, padded_height),(padded_width, padded_width)),'constant',constant_values = 0).astype(np.float32)

    height, width =  padded_array.shape
    height = height - (filter_height - 1) # 원래대로
    width = width - (filter_width - 1)

    # new 배열 생성
    filtered_array = np.empty((height, width), dtype = np.float32)

    # Convolution
    for i in range(height):
        for j in range(width):
            # Extract region of interest
            region = padded_array[i:i+filter_height, j:j+filter_width]
            # Element-wise multiplication and summation
            filtered_array[i, j] = np.sum(region * filter)

    return filtered_array

def gaussconvolve2d(array, sigma):
    # 2D 가우스 필터 생성
    gaussian_filter = gauss2d(sigma).astype(np.float32)
    # 필터 적용
    filtered_array = convolve2d(array, gaussian_filter)
    return filtered_array.astype(np.uint8)

def reduce_noise(img):
    sigma = 1.6
    #gray scale로 변환
    gray_img = img.convert('L')
    img_array = np.asarray(gray_img)

    # sigma =1.6 으로 설정해 필터 적용
    filtered_array = gaussconvolve2d(img_array, sigma)
    filtered_image = Image.fromarray(filtered_array)

    # 블러 이미지로 저장 
    filtered_image.save('b_ghost.jpeg')
    filtered_image.show()

    return filtered_array

img = Image.open('ghost.jpeg')
img.show()
reduce_noise(img)


def sobel_filters(img):
    # x 및 y 방향으로 Sobel 필터를 적용
    sobel_x = np.array([[1, 0, -1], [2, 0, -2], [1, 0, -1]])
    sobel_y = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])

    # convolve2d 함수를 사용하여 x 및 y 방향의 그래디언트를 계산
    gradient_x = convolve2d(img, sobel_x)
    gradient_y = convolve2d(img, sobel_y)

    # 경사도 크기를 계산
    G = np.hypot(gradient_x, gradient_y)
    # 경사도 크기를 0~255 범위로 매핑
    G = G * 255 / np.max(G)

    # 경사도 방향을 계산
    theta = np.arctan2(gradient_y, gradient_x)

    return (G, theta)

img = Image.open('b_ghost.jpeg')
img_array = np.array(img)
G, theta = sobel_filters(img_array)
G = Image.fromarray(G)
G.show()


def non_max_suppression(G, theta):
    # 결과 이미지 초기화
    res = np.zeros_like(G)
    # 이미지의 높이와 너비 구하기
    height, width = G.shape

    # 각 픽셀을 돌면서 Non-Maximum Suppression 수행
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            angle = np.degrees(theta[i, j])  # 라디안에서 도로 변환
            if (angle < 0) :
                angle = angle + 180 # 음수 예외 처리
            
            # 각도를 0, 45, 90, 135도로 구분하여 처리한다.           
            if (angle >= 0 and angle < 22.5) or (angle >= 157.5 and angle <= 180): # 좌우 비교
                q = G[i, j + 1]
                r = G[i, j - 1]
            elif (angle >= 22.5 and angle < 67.5) : # 좌측 아래, 우측 위 비교
                q = G[i - 1, j - 1]
                r = G[i + 1, j + 1]
            elif (angle >= 67.5 and angle < 112.5) : # 위아래 비교
                q = G[i - 1, j]
                r = G[i + 1, j]
            else: # 112.5 ~ 157.5 사이 -> 좌측 위, 우측 아래 비교
                q = G[i - 1, j + 1]
                r = G[i + 1, j - 1]

            # 현재 픽셀의 값과 이웃한 두 픽셀의 값을 비교하여 Non-Maximum Suppression 수행
            if G[i, j] >= q and G[i, j] >= r:
                res[i, j] = G[i, j]
            else:
                res[i, j] = 0

    return res

G = np.array(G)
nms_img = non_max_suppression(G, theta)
nms_img = nms_img.astype('uint8')
nms_img = Image.fromarray(nms_img)
nms_img.show()



def double_thresholding(img):
    # nms 결과를 세 가지 카테고리로 분류 (strong / weak / non-relevant)
    diff = img.max() - img.min()
    T_high = np.min(img) + diff * 0.15  # 높은 임계값 계산
    T_low = np.min(img) + diff * 0.03   # 낮은 임계값 계산

    # np.where() 함수를 사용하여 픽셀을 강한, 약한, 관련 없는 픽셀로 분류
    res = np.where(img < T_low, 0, img) # non_relevant -> 0으로 지정
    res = np.where(((T_low <= img) & (img < T_high)), 80, res) # weak -> 80으로 지정
    res = np.where(T_high <= img, 255, res) # strong -> 255로 지정
    return res

nms_img_array = np.array(nms_img) # 배열로 전달해야 오류 x
dt_img_array = double_thresholding(nms_img_array)
dt_img = Image.fromarray(dt_img_array.astype('uint8'))
dt_img.show()



def dfs(img, res, i, j, visited=[]):
    res[i, j] = 255

    # mark the visitation
    visited.append((i, j))

    # examine (i, j)'s 8 neighbors
    # call dfs recursively if there is a weak edge
    for ii in range(i-1, i+2) :
        for jj in range(j-1, j+2) :
            if (img[ii, jj] == 80) and ((ii, jj) not in visited) :
                dfs(img, res, ii, jj, visited)

def hysteresis(img):
    height, width = img.shape # 이미지 높이, 너비 가져옴
    res = np.array(img) # image -> array 배열 변환
    
    # 강한 픽셀의 부분을 찾아 저장
    strong_pixel = np.where(res == 255)

    # dfs 함수 사용을 위해 방문 여부를 표기하는 2차원 배열 생성
    visited = [[False] * width for _ in range(height)]

    # 강한 픽셀을 순회하며 약한 픽셀의 연결된 부분을 찾는다
    strong_pixel = np.transpose(strong_pixel)
    for i, j in strong_pixel:
        dfs(img, res, i, j, visited)
    
    # 다 돌았으면 약한 픽셀은 0으로 처리해 비연결 약한 픽셀 제거함
    weak_pixel = np.where(res == 80)
    res[weak_pixel] = 0

    return res

dt_img_array = np.array(dt_img)
hys_img_array = hysteresis(dt_img_array)
hys_img = Image.fromarray(hys_img_array.astype('uint8'))
hys_img.show()


def main():
    RGB_img = Image.open('./iguana.bmp')

    noise_reduced_img = reduce_noise(RGB_img)
    Image.fromarray(noise_reduced_img.astype('uint8')).save('./iguana_blurred.bmp', 'BMP')
    
    g, theta = sobel_filters(noise_reduced_img)
    Image.fromarray(g.astype('uint8')).save('./iguana_sobel_gradient.bmp', 'BMP')
    Image.fromarray(theta.astype('uint8')).save('./iguana_sobel_theta.bmp', 'BMP')

    non_max_suppression_img = non_max_suppression(g, theta)
    Image.fromarray(non_max_suppression_img.astype('uint8')).save('./iguana_non_max_suppression.bmp', 'BMP')

    double_threshold_img = double_thresholding(non_max_suppression_img)
    Image.fromarray(double_threshold_img.astype('uint8')).save('./iguana_double_thresholding.bmp', 'BMP')

    hysteresis_img = hysteresis(double_threshold_img)
    Image.fromarray(hysteresis_img.astype('uint8')).save('./iguana_hysteresis.bmp', 'BMP')

#main()