import numpy as np
import cv2

# 读取图像
filename = input("输入文件路径：")
# 避免编码格式不同导致文件读取失败，先用numpy读一下
img = cv2.imdecode(np.fromfile(filename, dtype=np.uint8), -1)
# img = cv2.resize(img, (432, 768), cv2.INTER_AREA)
K = int(input("输入聚类数目："))
print(img.shape)

# 将图像信息转化为一维
data = img.reshape((-1, 3))
# 将图像信息转化为float32型以便聚类
data = np.float32(data)

# 聚类，返回值为紧密度、结果标记数组、由聚类中心组成的数组
compactness, labels, centers = cv2.kmeans(
    data,  # 数据集
    K,  # 聚类数目
    None,
    (
        cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER,
        10,
        1.0,
    ),  # 终止条件(type，max_iter,epsilon)
    10,  # 重复试验kmeans算法次数
    cv2.KMEANS_RANDOM_CENTERS,  # 初始中心
)

# 将图像转换回uint8类型
centers = np.uint8(centers)
# 填充颜色
temp = centers[labels.reshape(-1, 1)]
# 显示与保存结果
result = temp.reshape((img.shape))
cv2.namedWindow("origin", cv2.WINDOW_NORMAL)
cv2.resizeWindow("origin", 572, 840)
cv2.imshow("origin", img)
cv2.namedWindow("result", cv2.WINDOW_NORMAL)
cv2.resizeWindow("result", 572, 840)
cv2.imshow("result", result)
cv2.imwrite("result.tiff", result)
cv2.waitKey()
