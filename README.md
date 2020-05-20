# Algorithm-Project

## 알고리즘 과제 - 비행기 예약 시스템

### Requirements
- Input: name (string of alphabets with arbitrary length), source, destination, date.  
- Output: name, reservation number, source, destination, departure time and date, flight path, arrival time and date, flight time (excluding wait time for transfer).
  - Where arrival time and date should be based on the shortest time, and flight path should include the intermediate cities in case of indirect path.  


▶ Reservation record: red black tree (rb-tree)
- Reservations should be inserted into the rb-tree with proper key (reservation number) order.
- A node contains all the necessary information regarding the reservation.
- A final flight path from a source to a destination can either the direct path or indirect path
(also there can be no path).
- A flight time between any two cities is determined by the locations.
Assume the airplane speed is 500 km/hour.
- Build initial rb-tree that contains 500 reservations (nodes).  


▶ Airline path: Graph data structure with adjacency list
- Shows bidirectional direct paths among cities (26 cities).
- A city name is a single alphabet character (from a ~z).
- Randomly generate 100 distinct direct paths between two cities, that is, both source city and destination city are randomly selected.
- A city location is randomly generated and represented by (x, y), where 0 kms <= x <=W3000 km or E3000 km, 0 kms <= y <=N3000 km or S3000 km,
- The graph consists of edges with distances (unit is km).


▶ Departure time table for one month (1st ~ 31st): Select a data structure of your choice.  
- For all the cities and all the direct paths, the departure times are randomly generated everyday for one month.
- A single departure time for a path. So there can be multiple departure times ifmultiple paths were generated for a city.
- The table contains one month schedule of all the cities departure times.


▶ Execution:
- Addition of reservations (at least ten insertions into the rb-tree) and deletion of reservations (at least ten actual deletions from the rb-tree, deletion by the key) should be executed for testing the system.
- Insertion test (print before & after the insertion): # of nodes, height of the tree
- Deletion test (print before & after the deletion): # of nodes, height of the tree

### Description of the system objective
- 프로그램을 실행하면 자동으로 26개의 도시와 각각의 좌표가 생성된다. 이 도시들을 연결할 수 있는 모든 경우의 수(325개) 중 랜덤하게 100개를 선정해 비행 경로로 설정한다. 그 후 각 루트에 출발 시간을 랜덤하게 생성한다. 그 후 500개의 랜덤한 여행 예약 레코드를 생성한다.
이 후 사용자 UI가 출력되는데 1번은 예약, 2번은 예약 정보 조회, 3번은 예약 취소, 4번은 나가기 기능이다. 
- #1: 예약 기능/이름, 출발지, 도착지, 출발 시간, 출발 날짜, 등급을 입력하면 가장 가까운 출발 날짜, 시간에 최단 경로로 가는 비행기를 예매해 준다. 만약 비행기 표가 없다면 갈 수 없다는 메세지가 뜨고 예약이 성공적으로 끝나면 랜덤으로 생성되는 예약번호와 비행시간 및 좌석등급에 따라 측정되는 가격, 루트와 함께 예약정보가 출력된다.   
- #2: 예약 정보 조회/예약번호를 입력하면 존재하는 예약번호면 예약정보가 출력되고, 존재하지 않는 예약번호면 일치하는 예약번호가 없다는 메시지가 출력된다.  
- #3: 예약 취소/예약번호를 입력하면 존재하는 예약번호면 정말로 삭제하겠냐고 묻는 메시지와 함께 (Y/N) 을 고르는 메시지가 출력된다. 이후 y를 택하면 삭제완료 메시지가 출력되고 올바르지 않은 예약번호를 입력했다면 다시 입력하라는 메시지가 출력된다.  
- #4: 프로그램 종료  
