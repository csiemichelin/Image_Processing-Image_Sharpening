# Image_Processing-Image_Sharpening
## 目錄
 - [Data_handed_in](#Data_handed_in)
 - [Technical_description](#Technical_description)
 - [Experimental_results](#Experimental_results)  
## Data_handed_in  
**1. Laplacian operator 資料夾**  
&emsp;Laplacian operator.cpp  
&emsp;Laplacian operator.exe  
&emsp;blurry_moon.tif    
&emsp;skeleton_orig.bmp     
**2. unsharp masking 資料夾**  
&emsp;unsharp masking.cpp  
&emsp;unsharp masking.exe  
&emsp;blurry_moon.tif    
&emsp;skeleton_orig.bmp   
**3. high-boost filtering 資料夾**  
&emsp;high-boost filtering.cpp  
&emsp;high-boost filtering.exe  
&emsp;blurry_moon.tif    
&emsp;skeleton_orig.bmp  
## Technical_description  
**1. Laplacian operator**   
&emsp;(1) 目的 : 與 smooth spatial filters 相反，主要用在 edge detection 上，也就是把影像中，物體的邊界找出來，利用二階微分實現 Laplacian operator。我所使用的 Laplacian mask 有以下兩種:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/1.png" width="230" height="120">     

&emsp;&emsp;故選取 center 為負的 image enhancement basic Laplacian operator 去做，使用雙層迴圈完成上面的公式，實現影像銳化的效果，且用 saturate_cast 函數防止資料溢位。    
&emsp;(2) 公式 :      
&emsp;&emsp;I. Space domain:   
&emsp;&emsp;&emsp;對影像微分，指的就是影像的像素值相減，再進行二次微分得到:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/2.png" width="230" height="110">      
&emsp;&emsp;&emsp;最後代入 Laplacian operator 得到:  
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/3.png" width="270" height="80">      
&emsp;&emsp;&emsp;而在 image enhancement 上使用的 basic Laplacian operator 為:    
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/4.png" width="270" height="90">      
&emsp;&emsp;&emsp;而這裡通常是用在銳化圖像上，故我們選取 center 為正的去做，其四鄰域化簡後的一般式為:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/5.png" width="330" height="90">      
&emsp;&emsp;II. Frequency domain:   
&emsp;&emsp;&emsp;對 n 階偏微分進行 DFT 來做推倒:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/6.png" width="250" height="130">    
&emsp;&emsp;&emsp;得到 Laplacian of f(x,y)為:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/7.png" width="180" height="35">    
&emsp;&emsp;&emsp;其 Filter 為:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/8.png" width="130" height="35">          
**2. unsharp masking**    
&emsp;(1) 目的 : 將原圖扣除模糊的部分(blurred version)得到銳化的部分，再銳化的部分加回原圖。以 space filter 為例，我所使用的 average filter mask 為以下兩種:  
&emsp;&emsp;先對其做 smooth spatial filter(此為 average filter)，也就是將所有權種加以來再進行平均，再將原圖扣掉其結果即為銳化的部分再將原圖加回銳化則可完成 sharping。   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/9.png" width="260" height="120">  
&emsp;(2) 公式 :  
&emsp;&emsp;I. Space domain:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/10.png" width="130" height="30">得到銳化的圖。  
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/11.png" width="40" height="25">  為 blurred version of f(x,y)，這裡我們使用 average filter 去實現。  
&emsp;&emsp;II. Frequency domain:     
&emsp;&emsp;&emsp;與 space domain 原理大致相同，但這裡的 low pass filter 採用 gaussian filter:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/12.png" width="150"   height="50">   
&emsp;&emsp;&emsp;再將原 gray level 扣除 gaussian low pass filter 的結果:  
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/13.png" width="160" height="24">    
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/14.png" width="160" height="25">   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/15.png" width="160" height="25">   
**3. high-boost filtering**   
&emsp;(1) 目的 : 與 unsharp masking 類似，不過在減去 blurred version 前，原影像會先乘上 A 倍且 A>=1。  
&emsp;&emsp;以 space filter 為例，鈍化遮罩：以本身影像減一個較為模糊之影像產生:   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/16.png" width="160" height="26">  
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/11.png" width="40" height="25">為模糊的影像，把遮罩以一個權重值加回原本影像   
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/17.png" width="160" height="24">  
&emsp;&emsp;其中 k = 1 的時候為 unsharp masking filter，k > 1 時為 high-boost filtering。   
&emsp;(2) 公式 :     
&emsp;&emsp;I. Space domain: A>=1    
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/18.png" width="280" height="80">  
&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/tech_image/19.png" width="165" height="24">  
&emsp;&emsp;II. Frequency domain: A>=1，這裡的 low pass filter 採用 gaussian filter    
## Experimental_results
&emsp;(1) 前置作業 :      
&emsp;&emsp;(一) 作業系統 : Windows 10 家用版     
&emsp;&emsp;(二) Visual studio 版本 : Visual Studio 2019     
&emsp;&emsp;(三) OpenCV 版本 : OpenCV 4.5.3     
&emsp;&emsp;(四) 環境架設 :        
&emsp;&emsp;&emsp;https://forum.gamer.com.tw/C.php?bsn=60292&snA=6494    
&emsp;(2) Laplacian operator(左:原圖，中:space filter，右:frequency filter)    
&emsp;&emsp;(一) blurry_moon.tif   
&emsp;&emsp;&emsp;&emsp;<img src="https://github.com/csiemichelin/Image_Processing-Image_Sharpening/blob/main/res_image/1.png" width="450" height="180">  
&emsp;&emsp;(二) skeleton_orig.bmp   
&emsp;(3) unsharp masking(左:原圖，中:space filter，右:frequency filter)     
&emsp;&emsp;(一) blurry_moon.tif   
&emsp;&emsp;(二) skeleton_orig.bmp  
&emsp;(4) high-boost filtering(左:原圖，中:space filter，右:frequency filter)     
&emsp;&emsp;(一) blurry_moon.tif   
&emsp;&emsp;(二) skeleton_orig.bmp  
