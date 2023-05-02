# 06 - Multiple Access and MAC

# CSMA (Carrier Sense Multiple Access)

- 누가 말을 하면 할 때까지 기다리는 것
- 기존 파형을 변조시킴.
1. 아무도 말 하고 있지 않으면 그냥 보낸다
2. 만약 누군가 말을 하고 있다면, (persistent : 확률, 시간이 지속적인지를 뜻함)
    - **1 Persistent-CSMA**
        - 채널이 busy한지 아닌지 계속 보다가, 끝나자마자 1의 확률로 보냄.
        - but, 여러 사람이 대기할 수 있으므로 충돌 발생 가능
    - **p Persistent-CSMA**
        - 채널이 busy한지 아닌지 계속 보다가, 끝나자마자 p의 확률로 전송함.
        - 하나 이상의 대기자가 접속할 때 충돌을 피할 수 있음.
    - **NP-CSMA**
        - 더 이상 듣지 않음.
        - random delay를 사용해 busy하지 않으면 보내겠다. (constant or variable delay)
    
    ![스크린샷 2023-05-02 오후 5.56.38.png](06%20-%20Multiple%20Access%20and%20MAC%20727fc56349bb4f9a99c07f114296baaa/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2023-05-02_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%2592%25E1%2585%25AE_5.56.38.png)
    

### CSMA Collisions

![스크린샷 2023-05-02 오후 6.01.13.png](06%20-%20Multiple%20Access%20and%20MAC%20727fc56349bb4f9a99c07f114296baaa/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2023-05-02_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%2592%25E1%2585%25AE_6.01.13.png)

- Collisions(충돌)은 여전히 발생
    - 전송자 사이 **Propagation delay** ≠ 0,
    - 충돌 발생 시
        - 전체 패킷 전송 시간이 **낭비!!**
    - 거리, 전파 지연 → 충돌 확률 결정

# Network Delay - 예시

- Ex) We have a water pipe with 100Km long and having a capacity of flowing 2 tons of water in 1 minutes. The speed of water in the pipe is 20km/hour. We want to transmits 100 tons of water using the pipe. Estimate the total delay to transmits 100 tons of water ?
    - 1분에 물 2톤 (관의 굵기)
    - 1시간에 물 20km (유속)
    
    → 100톤, 관 100km일때 물 보내면 Total Delay는?
    
    Ans : 한 시간당 20km씩 100km 가려면 첫 번째 물방울이 관 통과시 5시간 걸림 + 마지막 물방울이 마지막 관 통과 시 50분 걸림
    
- **전송 지연 (Transmission Delay)**
    - 용량 (파이프의 굵기와 관련)
- **전파 지연 (Propagation Delay)**
    - 신호의 속도 (링크의 거리와 관련)

→ 요약 : propagation delay보다, transmission delay가 대부분의 경우 더 크게 발생함! (데이터의 용량이 중요하다)

# CSMA/Collision Detection

- 충돌 피해를 감소시키자!
    
    ![스크린샷 2023-05-02 오후 6.30.55.png](06%20-%20Multiple%20Access%20and%20MAC%20727fc56349bb4f9a99c07f114296baaa/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2023-05-02_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%2592%25E1%2585%25AE_6.30.55.png)
    
- Keep listening to channel
    - 내가 말을 하면서도 다른 사람이  말하는지 계속 들음
- If (Transmitted_Signal ≠ Sensed_Signal)
    - Sender는 충돌임을 안다!
    - ABORT (끝냄)

### CSMA/CD에 대한 두 가지 관측

- 전송자는 동시에 send/listen이 가능
    - sensed-received = null이면 성공!
- 송신기, 수신기에서의 신호가 동일 (분산x)
- 유선에서는 신호의 에너지가 대부분 유지되지만, 무선에서는 신호의 에너지가 급격히 약해짐
    
    → 무선 통신에서는 CSMA/CD가 어려움..
    

# 무선 통신에서의 신호 분산(약화)

- 거리가 멀어짐에 따라 신호 세기가 약화됨
- SINR threshold 밑으로 내려가면 들을 수 없다..
- send/listen이 병렬적으로 이뤄질 수 없음 → Collision Detection이 어렵다!
- 전송자는 자기 자신것만 들음
- 수신자에서의 신호 품질을 결정할 수 없다!
    - 충돌은 수신자 입장에서 봐야 함.
    - Wi-Fi는 그래서 CA(Collision Avoidance) 사용.

## 정리하자면

무선 통신의 근본적 문제 두가지

1. **HTP(Hidden Terminal Problem)**
    - **존재를 모르기 때문에 자원 낭비**
    - 중간 애한테 양쪽 애가 신호 보냄. (양쪽 애들은 서로 SINR보다 낮은 두 개였어서 못 알아채고) 근데 중간 애는 충돌 발생함..
2. **ETP(Exposed Terminal Problem)** 
    - **존재를 알기 때문에 효율 저하**
    - 신호 보내고 싶은데.. 다른 애가 수신자한테 신호 보내는것 보고 안 보냄 (근데 다른 애도 어차피 SINR보다 낮아서 못 보는 신호였음)
- 통합 정리
    
    ![스크린샷 2023-05-02 오후 6.44.58.png](06%20-%20Multiple%20Access%20and%20MAC%20727fc56349bb4f9a99c07f114296baaa/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2023-05-02_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%2592%25E1%2585%25AE_6.44.58.png)
    

# “Taking Turns” Mac Protocols

[**배경**]

- MAC 프로토콜에서의 채널 분할
    - high load → 채널을 효율적이고 공평하게 나눔
    - low load → 비효율적. 채널 접근 delay, bandwidth 1/N 이니까 활성 노드 하나만 있어도 할당됨
- MAC 프로토콜에서의 Random Access
    - 부하가 낮은 상태에서 효과적으로 채널 사용이 가능함
    - 근데 부하 많아지면 충돌 증가 증가 ..
- “Taking Turns” 프로토콜
    - 두 가지 방법 장점 살림

**[Polling]**

![스크린샷 2023-05-02 오후 6.52.52.png](06%20-%20Multiple%20Access%20and%20MAC%20727fc56349bb4f9a99c07f114296baaa/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2023-05-02_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%2592%25E1%2585%25AE_6.52.52.png)

- master 노드가 slave 노드들한테 하나 하나 순서대로 물어봄
- slave 장치 → dumb
- 단점
    - polling overhead
    - latency
    - single point of failure (마스터 문제 생기면 전체 동작 못함)

**[Token Passing]**

![스크린샷 2023-05-02 오후 6.53.03.png](06%20-%20Multiple%20Access%20and%20MAC%20727fc56349bb4f9a99c07f114296baaa/%25E1%2584%2589%25E1%2585%25B3%25E1%2584%258F%25E1%2585%25B3%25E1%2584%2585%25E1%2585%25B5%25E1%2586%25AB%25E1%2584%2589%25E1%2585%25A3%25E1%2586%25BA_2023-05-02_%25E1%2584%258B%25E1%2585%25A9%25E1%2584%2592%25E1%2585%25AE_6.53.03.png)

- token은 하나만 존재함. token 가진 애만 데이터 보낼 수 있음
- 802.5 등 FDDI에 사용
- 단점
    - token overhead
    - latency
    - single point of failure (토큰 잃어버리면 끝~)