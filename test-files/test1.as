; this is commant
MAIN:            add      r3, LIST 
LOOP: prn #48 
mcro  add as
cmp     K, #-6222 
   bne &END 
mcroend  1
 lea STR, r6 
 inc r6 

    


                                mov         r3, K 
 sub r1, r4 
 bne END 
a_mc 
  dec K 
 jmp &LOOP 
 END: stop 
STR: .string “abcd” 
LIST: .data 6, -922 
                       .data    -100 
K: .data 31 

;שם של הוראה\הנחיה לא יכול להיות שם של מאקרו
;בשורת ההגדרה (mcro) ובשורת הסיום (mcroend) אין תווים נוספים
;אם נמצאה שגיאה בשלב פריסת המקרו אי אפשר לעבור לשלב הבא
;יש לעצור, להודיע על השגיאה ולעבור לקובץ הבא
; אורכה של שורה בקובץ AS הוא לכל היותר 80 תווים ללא התו \N

 