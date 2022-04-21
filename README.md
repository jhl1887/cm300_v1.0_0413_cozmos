<!--
***********************************
*** 1. Company : cozmos         ***
*** 2. Edit_date : 2022-04-21   ***
*** 3. Editer : JHLee           ***
***********************************
-->


# CM300_Release_Note

***

* ## version_check
<table>
<thead>
  <tr>
    <th>버전</th>
    <th>날짜</th>
    <th>변경이력</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td rowspan="9">V.1.0</td>
    <td rowspan="9">2022.04.13</td>
    <td>문서 최초 작성</td>
  </tr>
  <tr>
    <td> - [New] 시나리오 소개와 관련 기술 상세 설명 (README_intro.md 파일 참조)</td>
  </tr>
  <tr>
    <td>　</td>
  </tr>
  <tr>
    <td>릴리즈 노트 Simple&nbsp;&nbsp;&nbsp;버전 작성</td>
  </tr>
  <tr>
    <td> - [Feature] Power_On Sound와 Func시작 Sound 사이에 delay_time 1sec 코드 추가</td>
  </tr>
  <tr>
    <td> - [Changed] Power_On Sound - '도레미파솔' -&gt; '도미솔'로 변경</td>
  </tr>
  <tr>
    <td>　</td>
  </tr>
  <tr>
    <td>S/W bug Check</td>
  </tr>
  <tr>
    <td> - [bug] Button 반복 press 시 전원 Off되는 현상 (Test中)</td>
  </tr>
</tbody>
</table>


***

* ## (참조) 제품 시나리오 표

<table>
<thead>
  <tr>
    <th>No.</th>
    <th>item</th>
    <th>Power_On</th>
    <th>&lt;- delay_time&nbsp;&nbsp;&nbsp;-&gt;</th>
    <th>Func_01~08</th>
    <th>Func_09~10</th>
    <th>Power_Off_Wait</th>
    <th>Power_Off</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>1</td>
    <td>key_press_time</td>
    <td>0.5sec</td>
    <td rowspan="4">1sec</td>
    <td>0.1sec</td>
    <td>0.1sec</td>
    <td colspan="2">1sec</td>
  </tr>
  <tr>
    <td>2</td>
    <td>led_time</td>
    <td>상시On</td>
    <td>blink&nbsp;&nbsp;&nbsp;2회(0.5sec)<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -&gt; On</td>
    <td>blink&nbsp;&nbsp;&nbsp;2회(0.5sec)<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -&gt; On</td>
    <td>On</td>
    <td>Off</td>
  </tr>
  <tr>
    <td>3</td>
    <td>sound_duty</td>
    <td>도미솔</td>
    <td>도레미파솔라시도</td>
    <td>09 - 도미솔 / 10 - 솔미도</td>
    <td>솔파미레도</td>
    <td>Off</td>
  </tr>
  <tr>
    <td>4</td>
    <td>motor_duty</td>
    <td>FLASH_ReadByte<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Func_마지막 저장 상태 사용)</td>
    <td>42,46,50,54,58,62,66,70</td>
    <td>[50,55,60,65,70,65,60,55,50]<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;100 - 0.1sec마다 idx++1<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;30 - 0.03sec마다 idx++1</td>
    <td>(current_motor_duty)</td>
    <td>Off</td>
  </tr>
</tbody>
</table>