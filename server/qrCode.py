from MyQR import myqr
import os

device = [1,3,5,7,13,20,23]

for idx in device:
    myqr.run(words = 'http://127.0.0.1:5000/index/%d' % (idx), # 可放網址或文字(限英文)
            picture = 'fire.jpg',
            version = 15, # QR Code的邊長，越大圖案越清楚
            level = 'H', # 糾錯水平，預設是H(最高)
            colorized = False, # 背景圖片是否用彩色，True為彩色
            save_name = os.path.join('qrcode', 'qrcode%d.jpg'% (idx))) # 儲存檔案名稱
