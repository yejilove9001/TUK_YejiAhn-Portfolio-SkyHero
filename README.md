#  Sky Hero – 방위탑 프로젝트

##  개요 (Overview)

본 프로젝트는 전시 상황을 가정하여, 공중에서 접근하는 위험 물체를 감지하고 추적하는 **공중 위협 대응 시스템**입니다.
초음파 센서와 서보모터를 활용해 이동 물체를 실시간으로 추적하며, LCD를 통해 위치 정보를 표시하고, 위험이 감지되면 경고등과 경보음을 울리는 **방위탑 형태의 임베디드 시스템**입니다.

##  사용한 기술 및 부품

- Raspberry Pi
- 초음파 센서 (Ultrasonic Distance Sensor)
- Servo Motor
- LED, Buzzer
- LCD 디스플레이
- C언어 + 라즈베리파이 GPIO 라이브러리

##  주요 기능

- 공중 위협물(모형 물체) 거리 및 위치 감지
- Servo Motor를 통해 방향 추적
- LCD에 실시간 거리 출력
- LED 경고등 및 Buzzer 알림 시스템

##  시연 영상

 [유튜브 링크](https://youtube.com/shorts/kvcR_sab930?si=uVnZpm1onwIMfKVm)

##  실행 방법

1. 초음파 센서 및 서보모터, LCD를 Raspberry Pi에 연결
2. `SkyHero.c`를 빌드 및 실행
3. 이동하는 물체가 감지되면 자동 추적 및 알림 표시

##  제작자

- **안예지 (Yeji Ahn)**  
  Embedded Systems, Tech University of Korea  
  Email: yejilove9001@naver.com  
  GitHub: [yejilove9001](https://github.com/yejilove9001)
