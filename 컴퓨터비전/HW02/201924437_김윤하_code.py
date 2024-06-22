from PIL import Image
import numpy as np
import math

#   Part 1-1
#    - return n*n np.array
#    - the values of elements are uniform and the sum of elements is 1
def boxfilter(n):
    # 짝수 예외처리
    assert n%2 != 0, "dimension must be odd"
    # element가 1/(n*n)이고 크기가 n*n인 배열 반환
    box_filter = np.full((n,n), 1/(n*n)) 
    return box_filter
#print(boxfilter(3))
#print(boxfilter(4))
#print(boxfilter(7))


#   Part 1-2
#    - return 1d gaussian filter with a length of n
#    - where n is a smallest odd number grater than 6*sigma (sigma의 6배보다 큰 수 중 가장 작은 홀수)
def gauss1d(sigma):
    n = int(np.ceil(6 * sigma))  # 필터 길이 계산
    if n % 2 == 0:  # 홀수로 만들기
        n += 1
    center = n // 2  # 중심 인덱스 계산
    x = np.arange(0, n) - center  # 중심을 기준으로 좌표 설정
    gaus1d_filter = np.exp(-x**2 / (2 * sigma**2))  # 가우스 함수 계산 (문제 제시 kernel)
    gaus1d_filter /= gaus1d_filter.sum()  # 정규화
    return gaus1d_filter
#sigma = [0.3, 0.5, 1, 2]
#for i in sigma:
#    print(f'sigma value: {i}\n', gauss1d(i))
#    print('\n')


#   Part 1-3
#    - return 2d gaussian filter with a dimension of (n*n)
#    - use gauss1d function
def gauss2d(sigma):
    # 1차원 가우스 필터를 생성
    g1d = gauss1d(sigma)
    # 2D 가우스 필터 생성
    #gauss2d_filter = np.outer(g1d, np.transpose(g1d))
    gauss2d_filter = np.outer(g1d, g1d)
    return gauss2d_filter
#sigma = [0.5, 1]
#for i in sigma:
#    print(f'sigma value: {i}\n', gauss2d(i))
#    print("\n")


#   Part 1-4 (a)
#    - apply convolution operation with zero paddings
#    - return filtered array
def convolve2d(array, filter):
    # Filter dimensions
    filter_height, filter_width = filter.shape
    # Array dimensions
    array_height, array_width = array.shape
    # Output array
    filtered_array = np.zeros_like(array)

    # Pad array
    padded_array = np.pad(array, [(filter_height//2, filter_height//2), (filter_width//2, filter_width//2)], mode='constant', constant_values = 0).astype(np.float32)

    # Convolution
    for i in range(array_height):
        for j in range(array_width):
            # Extract region of interest
            region = padded_array[i:i+filter_height, j:j+filter_width]
            # Element-wise multiplication and summation
            filtered_array[i, j] = np.sum(region * filter)

    return filtered_array


#   Part 1-4 (b)
#    - apply gaussian filter with zero paddings
#    - use gauss2d and convolve2d functions
def gaussconvolve2d(array, sigma):
    # 2D 가우스 필터 생성
    gaussian_filter = gauss2d(sigma)
    # 필터 적용
    filtered_array = convolve2d(array, gaussian_filter)
    return filtered_array


#   Part 1-4 (c), (d)
#    - apply gaussian filter to '2d-dog.bmp' with sigma=3, implement part1_4()
#    - convert the RGB image to Gray image and apply filter
#    - save the blurred image
#    - use PIL library
def part1_4():
    img = Image.open('dog.bmp')

    #gray scale로 변환
    gray_img = img.convert('L')
    img_array = np.asarray(gray_img)

    # sigma =3으로 설정해 필터 적용
    filtered_array = gaussconvolve2d(img_array, 3)
    filtered_image = Image.fromarray(filtered_array)

    # 블러 이미지로 저장 
    filtered_image.save('blurred_dog.bmp')
    filtered_image.show()

    return filtered_array
#part1_4()


####################################################################################################


#   Part 2-1
#    - apply gaussian filter to each channel(R, G, B)
#    - return low frequency image array
def part2_1(image_path, sigma):
    image = Image.open(image_path)
    image.show()
    image_array = np.array(image, dtype=float)  # 이미지를 numpy 배열로 변환
    
    low_freq_array = np.zeros_like(image_array)  # 낮은 주파수 이미지를 저장할 배열 초기화
    
    # R, G, B 채널별로 가우시안 필터 적용
    for i in range(3):  # RGB 채널은 3개이므로 range(3) 사용
        low_freq_array[:, :, i] = gaussconvolve2d(image_array[:, :, i], sigma)
    

    return low_freq_array
#lfa = part2_1(image_path='images/3a_lion.bmp', sigma=4)
#Image.fromarray(lfa.astype(np.uint8)).show()


#   Part 2-2
#    - apply gaussian filter and make low frequency image and then subtract it from origin
#    - return high frequency image array
def part2_2(image_path, sigma):
    image = Image.open(image_path)
    image.show()
    image_array = np.array(image, dtype=float)
    
    high_freq_array = np.zeros_like(image_array)
    
    # R, G, B 채널별로 가우시안 필터 적용하여 낮은 주파수 이미지 생성 후 원본에서 빼서 고주파수 이미지 생성
    for i in range(3):
        low_freq = gaussconvolve2d(image_array[:, :, i], sigma)
        high_freq_array[:, :, i] = image_array[:, :, i] - low_freq + 128  # 시각화를 위해 128 추가
    
    return high_freq_array
#hfa = part2_2(image_path='images/3b_tiger.bmp', sigma=4)
#Image.fromarray(hfa.astype(np.uint8)).show()

#   Part 2-3
#    - make hybrid image using part2_1() and part 2_2()
def part2_3(sigma1, sigma2):
    image_path1 = 'images/3a_lion.bmp'
    image_path2 = 'images/3b_tiger.bmp'
    low_freq_array = part2_1(image_path1, sigma1)
    high_freq_array = part2_2(image_path2, sigma2) - 128  # 시각화를 위해 추가된 128을 제거
    
    hybrid_array = low_freq_array + high_freq_array  # 낮은 주파수 이미지와 고주파수 이미지의 합
    
    # 합산된 이미지의 픽셀 값 범위를 [0, 255]로 조정
    hybrid_array = np.clip(hybrid_array, 0, 255)
    
    hybrid_image = Image.fromarray(hybrid_array.astype('uint8'))  # numpy 배열을 이미지로 변환
    
    hybrid_image.show()
    return hybrid_image
#hi = part2_3(sigma1=8, sigma2=4)
