import numpy as np
import cv2
import math
import random


# 1-2.
def RANSACFilter(
        matched_pairs, keypoints1, keypoints2,
        orient_agreement, scale_agreement):
    """
    This function takes in `matched_pairs`, a list of matches in indices
    and return a subset of the pairs using RANSAC.
    Inputs:
        matched_pairs: a list of tuples [(i, j)],
            indicating keypoints1[i] is matched
            with keypoints2[j]
        keypoints1, 2: keypoints from image 1 and image 2
            stored in np.array with shape (num_pts, 4)
            each row: row, col, scale, orientation
        *_agreement: thresholds for defining inliers, floats
    Output:
        largest_set: the largest consensus set in [(i, j)] format

    HINTS: the "*_agreement" definitions are well-explained
           in the assignment instructions.
    """
    assert isinstance(matched_pairs, list)
    assert isinstance(keypoints1, np.ndarray)
    assert isinstance(keypoints2, np.ndarray)
    assert isinstance(orient_agreement, float)
    assert isinstance(scale_agreement, float)

    ## START
    largest_set = []

    num_matches = len(matched_pairs)

    for _ in range(10):
        # matched_pairs 리스트에서 무작위로 하나의 매치를 선택
        rand_idx = random.randrange(num_matches)
        rand_pair = matched_pairs[rand_idx]
        # 선택한 매치의 기준 orientation, scale을 계산
        ref_orientation = (keypoints1[rand_pair[0]][3] - keypoints2[rand_pair[1]][3]) % (2 * np.pi)
        ref_scale = keypoints2[rand_pair[1]][2] / keypoints1[rand_pair[0]][2]

        temp_list = []
        for i, pair in enumerate(matched_pairs):
            if i != rand_idx:
                # 현재 매치의 orientation, scale을 계산
                curr_orientation = (keypoints1[pair[0]][3] - keypoints2[pair[1]][3]) % (2 * np.pi)
                curr_scale = keypoints2[pair[1]][2] / keypoints1[pair[0]][2]

                # 현재 매치의 orientation, scale이 기준값과 일정한 오차 범위 내에 있는지 확인
                if abs(curr_orientation - ref_orientation) <= orient_agreement * np.pi / 180 and abs(curr_scale - ref_scale) <= scale_agreement:
                    # 조건을 만족하는 매치를 임시 리스트에 추가
                    temp_list.append(pair)

        # 현재 임시 리스트가 지금까지의 가장 큰 합의 집합보다 클 시 -> 가장 큰 합의 집합으로 설정
        if len(temp_list) > len(largest_set):
            largest_set = temp_list

    ## END
    assert isinstance(largest_set, list)
    return largest_set


# 1-1.
def FindBestMatches(descriptors1, descriptors2, threshold):
    # distance angle 계산 -> arccos (a*b)/|a||b| 이용해 계산하기
    # filtering -> Ratio distance = best math angle / second-best match angle
    # if (Radio distance > threshold) then discard matches
    # theta1/theta2 <= 1 -> 유사한 케이스의 경우임. 높으면 인라이어/아웃라이어든 버리고 사용 x (담벼락? 같은 예시)
    # threshold가 작으면 ambiguos를 다 버림 -> 컴팩트한 케이스만 남게 됨.
    # 반면, threshold가 높으면 ambigous가 많이 나오게 된다 -> threshold를 조절해 적당한 값 찾기
    """
    This function takes in descriptors of image 1 and image 2,
    and find matches between them. See assignment instructions for details.
    Inputs:
        descriptors: a K-by-128 array, where each row gives a descriptor
        for one of the K keypoints.  The descriptor is a 1D array of 128
        values with unit length.
        threshold: the threshold for the ratio test of "the distance to the nearest"
                   divided by "the distance to the second nearest neighbour".
                   pseudocode-wise: dist[best_idx]/dist[second_idx] <= threshold
    Outputs:
        matched_pairs: a list in the form [(i, j)] where i and j means
                       descriptors1[i] is matched with descriptors2[j].
    """
    assert isinstance(descriptors1, np.ndarray)
    assert isinstance(descriptors2, np.ndarray)
    assert isinstance(threshold, float)

    ## START
    ## the following is just a placeholder to show you the output format

    num = 5
    matched_pairs = [[i, i] for i in range(num)]

    y1 = descriptors1.shape[0]
    y2 = descriptors2.shape[0]

    temp = np.zeros(y2)
    matched_pairs = []

    # 이중 for문으로 각 d1에 대해 모든 d2값들을 계산해 temp에 저장, match point 찾기
    for i in range(y1): 
        for j in range(y2):
            temp[j] = math.acos(np.dot(descriptors1[i], descriptors2[j]))
        
        compare = sorted(range(len(temp)), key= lambda k : temp[k])
        if (temp[compare[0]] / temp[compare[1]]) < threshold:
            matched_pairs.append([i, compare[0]])
    
    ## END
    return matched_pairs

# 2-1.
def KeypointProjection(xy_points, h):
    # main_proj.py에서 작동 확인
    # H * (x, y) = (x', y')
    """
    This function projects a list of points in the source image to the
    reference image using a homography matrix `h`.
    Inputs:
        xy_points: numpy array, (num_points, 2)
        h: numpy array, (3, 3), the homography matrix
    Output:
        xy_points_out: numpy array, (num_points, 2), input points in
        the reference frame.
    """
    assert isinstance(xy_points, np.ndarray)
    assert isinstance(h, np.ndarray)
    assert xy_points.shape[1] == 2
    assert h.shape == (3, 3)

    # START
    # xy_points 배열에 1을 추가하여 homogeneous coordinates 만들기
    hc_xys = np.pad(xy_points, pad_width=((0, 0), (0, 1)), mode='constant', constant_values=1)
    xys_p = h @ hc_xys.T
    # 마지막 좌표가 0인 경우를 피하기 위해 0을 작은 값으로 대체
    z_cor = np.where(xys_p[-1,:] == 0, 0.0000001, xys_p[-1,:])
    hc_xys_p = xys_p/z_cor
    xys_p = hc_xys_p[:-1,:]
    return xys_p.T
    # END
    #return xy_points_out

# 2-2. Homography 계산
def RANSACHomography(xy_src, xy_ref, num_iter, tol):
    # xy_src : source 이미지에서의 x, y 값 [x1, y1], [x2, y2] ..
    # xy_ref : [x1', y1'] ... -> 계산값과 정답값이 일치하는지 확인이 가능함

    assert isinstance(xy_src, np.ndarray)
    assert isinstance(xy_ref, np.ndarray)
    assert xy_src.shape == xy_ref.shape
    assert xy_src.shape[1] == 2
    assert isinstance(num_iter, int)
    assert isinstance(tol, (int, float))
    tol = tol*1.0

    # START
    max_inliers = 0
    h = None
    N = xy_src.shape[0]

    for i in range(num_iter):
        sample_indices = np.random.choice(N, 4, replace = False) # 각각 4개의 페어를 이용함
        src_sample = xy_src[sample_indices]
        ref_sample = xy_ref[sample_indices]

        H = compute_homography(src_sample, ref_sample) # opencv를 이용하지 않고 구현

        xy_proj = KeypointProjection(xy_src, H)
        distances = np.sqrt(np.sum((xy_proj - xy_ref) ** 2, axis=1))
        inliers = np.sum(distances < tol)

        if inliers > max_inliers: # inlier가 가장 많았을 때의 H를 선택해 줌.
            best_h = H
            max_inliers = inliers

    return best_h

    # END
    assert isinstance(h, np.ndarray)
    assert h.shape == (3, 3)
    return h

# 2-2에 추가 (opencv 대신 numpy만 사용해서 구현)
def compute_homography(xy_src, xy_ref):
    assert xy_src.shape[0] == xy_ref.shape[0]
    assert xy_src.shape[0] == 4 # 최소 4개의 점이 필요

    A = []
    for i in range(xy_src.shape[0]):
        x_src, y_src = xy_src[i]
        x_ref, y_ref = xy_ref[i]
        A.append([-x_src, -y_src, -1, 0, 0, 0, x_ref*x_src, x_ref*y_src, x_ref]) # A 구성해주기
        A.append([0, 0, 0, -x_src, -y_src, -1, y_ref*x_src, y_ref*y_src, y_ref])
    
    A = np.array(A)
    U, S, Vh = np.linalg.svd(A) # 특이값 분해
    H = Vh[-1].reshape(3, 3)
    
    return H / H[2, 2] 


def FindBestMatchesRANSAC(
        keypoints1, keypoints2,
        descriptors1, descriptors2, threshold,
        orient_agreement, scale_agreement):
    """
    Note: you do not need to change this function.
    However, we recommend you to study this function carefully
    to understand how each component interacts with each other.

    This function find the best matches between two images using RANSAC.
    Inputs:
        keypoints1, 2: keypoints from image 1 and image 2
            stored in np.array with shape (num_pts, 4)
            each row: row, col, scale, orientation
        descriptors1, 2: a K-by-128 array, where each row gives a descriptor
        for one of the K keypoints.  The descriptor is a 1D array of 128
        values with unit length.
        threshold: the threshold for the ratio test of "the distance to the nearest"
                   divided by "the distance to the second nearest neighbour".
                   pseudocode-wise: dist[best_idx]/dist[second_idx] <= threshold
        orient_agreement: in degrees, say 30 degrees.
        scale_agreement: in floating points, say 0.5
    Outputs:
        matched_pairs_ransac: a list in the form [(i, j)] where i and j means
        descriptors1[i] is matched with descriptors2[j].
    Detailed instructions are on the assignment website
    """
    orient_agreement = float(orient_agreement)
    assert isinstance(keypoints1, np.ndarray)
    assert isinstance(keypoints2, np.ndarray)
    assert isinstance(descriptors1, np.ndarray)
    assert isinstance(descriptors2, np.ndarray)
    assert isinstance(threshold, float)
    assert isinstance(orient_agreement, float)
    assert isinstance(scale_agreement, float)
    matched_pairs = FindBestMatches(
        descriptors1, descriptors2, threshold)
    matched_pairs_ransac = RANSACFilter(
        matched_pairs, keypoints1, keypoints2,
        orient_agreement, scale_agreement)
    return matched_pairs_ransac
