<!--
***********************************
*** 1. Company : cozmos         ***
*** 2. Edit_date : 2022-04-18   ***
*** 3. Editer : JHLee           ***
***********************************
-->


# CM300_Release Note_22.04.21

***

* ## cm300_v1.0_0413
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-cly1{text-align:left;vertical-align:middle}
.tg .tg-oiky{color:#0070C0;font-weight:bold;text-align:left;vertical-align:middle}
.tg .tg-ukly{background-color:#D9D9D9;font-weight:bold;text-align:center;vertical-align:middle}
.tg .tg-nrix{text-align:center;vertical-align:middle}
.tg .tg-0lax{text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-ukly">버전</th>
    <th class="tg-ukly">날짜</th>
    <th class="tg-ukly">변경이력</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-nrix" rowspan="9">V.1.0</td>
    <td class="tg-nrix" rowspan="9">2022.04.13</td>
    <td class="tg-oiky">문서 최초 작성</td>
  </tr>
  <tr>
    <td class="tg-0lax"> - [<span style="color:#3166FF">New</span>] 시나리오 소개와 관련 기술 상세 설명 (README_intro.md 파일 참조)</td>
  </tr>
  <tr>
    <td class="tg-cly1">　</td>
  </tr>
  <tr>
    <td class="tg-oiky">릴리즈 노트 Simple&nbsp;&nbsp;&nbsp;버전 작성</td>
  </tr>
  <tr>
    <td class="tg-0lax"> - [<span style="color:#32CB00">Feature</span>] Power_On Sound와 Func시작 Sound 사이에 delay_time 1sec 코드 추가</td>
  </tr>
  <tr>
    <td class="tg-0lax"> - [<span style="color:#F8A102">Changed</span>] Power_On Sound - '도레미파솔' -&gt; '도미솔'로 변경</td>
  </tr>
  <tr>
    <td class="tg-cly1">　</td>
  </tr>
  <tr>
    <td class="tg-oiky">S/W bug Check</td>
  </tr>
  <tr>
    <td class="tg-0lax"> - [<span style="color:#FE0000">bug</span>] Button 반복 press 시 전원 Off되는 현상 (Test中)</td>
  </tr>
</tbody>
</table>

***

* ## (참조) 제품 시나리오 표

<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-wa1i{font-weight:bold;text-align:center;vertical-align:middle}
.tg .tg-ukly{background-color:#D9D9D9;font-weight:bold;text-align:center;vertical-align:middle}
.tg .tg-nrix{text-align:center;vertical-align:middle}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-ukly">No.</th>
    <th class="tg-ukly">item</th>
    <th class="tg-ukly">Power_On</th>
    <th class="tg-ukly">&lt;- delay_time -&gt;</th>
    <th class="tg-ukly">Func_01~08</th>
    <th class="tg-ukly">Func_09~10</th>
    <th class="tg-ukly">Power_Off_Wait</th>
    <th class="tg-ukly">Power_Off</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-wa1i">1</td>
    <td class="tg-nrix">key_press_time</td>
    <td class="tg-nrix">0.5sec</td>
    <td class="tg-nrix" rowspan="4">1sec</td>
    <td class="tg-nrix">0.1sec</td>
    <td class="tg-nrix">0.1sec</td>
    <td class="tg-nrix" colspan="2">1sec</td>
  </tr>
  <tr>
    <td class="tg-wa1i">2</td>
    <td class="tg-nrix">led_time</td>
    <td class="tg-nrix">상시On</td>
    <td class="tg-nrix">blink 2회(0.5sec)<br> -&gt; On</td>
    <td class="tg-nrix">blink 2회(0.5sec)<br> -&gt; On</td>
    <td class="tg-nrix">On</td>
    <td class="tg-nrix">Off</td>
  </tr>
  <tr>
    <td class="tg-wa1i">3</td>
    <td class="tg-nrix">sound_duty</td>
    <td class="tg-nrix">도미솔</td>
    <td class="tg-nrix">도레미파솔라시도</td>
    <td class="tg-nrix">09 - 도미솔 / 10 - 솔미도</td>
    <td class="tg-nrix">솔파미레도</td>
    <td class="tg-nrix">Off</td>
  </tr>
  <tr>
    <td class="tg-wa1i">4</td>
    <td class="tg-nrix">motor_duty</td>
    <td class="tg-nrix">FLASH_ReadByte<br>(Func_마지막 저장 상태 사용)</td>
    <td class="tg-nrix">42,46,50,54,58,62,66,70</td>
    <td class="tg-nrix">[50,55,60,65,70,65,60,55,50]<br>100 - 0.1sec마다 idx++1<br>30 - 0.03sec마다 idx++1</td>
    <td class="tg-nrix">(current_motor_duty)</td>
    <td class="tg-nrix">Off</td>
  </tr>
</tbody>
</table>

***

