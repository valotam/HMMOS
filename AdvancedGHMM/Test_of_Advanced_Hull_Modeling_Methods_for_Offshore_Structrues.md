# Course: Advanced Hull Modeling Methods for Offshore Structures

- Date: October 23rd, 2018
- Subject: Midterm Exam (take-home exam)
- Due: 09:30 am, October 24th, 2018

% _On the first page of the answer sheet, write down the honor code, followed by your signature. Then give your answer for each problem after writing down the problem sentences._

Honor code: I have neither given nor received any aid from anyone on this take-home examination.  

`->`

--- 

1. Compare the bilinear surface with the ruled surface. Describe the characteristics of each surface representation.

**Bilinear surface**:

- 3차원 공간 상의 네 개의 꼭지점을 경계로 잡고 무게중심 보간(barycentric interpolation)하여 형성되는 곡면이다.

$$
\overrightarrow{s}(u,v) =
\begin{bmatrix}
    (1 - u) & u
\end{bmatrix}
\begin{bmatrix}
    \mathbf{P}(0, 0) & \mathbf{P}(0, 1) \\
    \mathbf{P}(1, 0) & \mathbf{P}(1, 1)
\end{bmatrix}
\begin{bmatrix}
    (1 - v) \\
    v
\end{bmatrix}
$$

- 항상 안장 형태의 곡면이다.
- 각 파라미터 방향으로 곡면 위에 선을 그으면 직선이된다.

**Ruled surface**:

- 파라미터 u와 v 방향 중에 한쪽 방향(예를 들어, u)의 경계 곡선 두 개를 다른 쪽 방향(예를 들어, v)으로 선형 보간(linear interpolation)하여 형성되는 곡면이다.

$$
\overrightarrow{s}(u,v) =
\begin{bmatrix}
    (1 - v) & v
\end{bmatrix}
\begin{bmatrix}
    \mathbf{P}(u, 0)    \\
    \mathbf{P}(u, 1)
\end{bmatrix}
$$

2. What are the properties of the linear Coons surface? What are the advantages and disadvantages of that representation?

**Linear Coons surface**:

- 네 개의 경계 곡선을 통해 형성되는 곡면으로, 이 곡면을 형성하는 방법은 다음과 같다.
- u 방향의 경계 곡선을 가지는 ruled surface와 v 방향의 경계 곡선을 가지고 있는 ruled surface를 합한 뒤, 이로 인해서 생기는 네 개의 경계 곡선과의 불일치함을, 네 개의 꼭지점을 경계로 하는 bilinear surface를 빼는 방법으로 제거하였다.

$$
\overrightarrow{s}(u,v) =
\begin{bmatrix}
    (1 - u) & u & 1
\end{bmatrix}
\begin{bmatrix}
    -\mathbf{P}(0, 0) & -\mathbf{P}(0, 1) & \mathbf{P}(0, v)    \\
    -\mathbf{P}(1, 0) & -\mathbf{P}(1, 1) & \mathbf{P}(0, 1)    \\
    \mathbf{P}(u, 0) & \mathbf{P}(u, 1) & 0
\end{bmatrix}
\begin{bmatrix}
    (1 - v) \\
    v       \\
    1
\end{bmatrix}
$$

- 네 개의 경계 곡선에 관한 식을 알면 곡면을 형성할 수 있다.
- 하지만, 경계 곡선이 곡면 중앙의 형상을 좌우하기 때문에 원하는 형상을 얻기 위해서 경계 곡선의 식을 조율하는 어려움이 존재한다.

3. Describe each term used in the definition of the NURBS surface.

$$
\overrightarrow{s}(u, v) =
\frac{
    \sum^{M}_{j = 0}{}
    \sum^{N}_{k = 0}{
        w_{j,k}
        \mathbf{P}_{j,k}
        \mathit{B}^{m}_{j}(u)
        \mathit{B}^{n}_{k}(v)
    }
}
{
    \sum^{M}_{j = 0}{}
    \sum^{N}_{k = 0}{
        w_{j,k}
        \mathit{B}^{m}_{j}(u)
        \mathit{B}^{n}_{k}(v)
    }
}
$$

- $w_{j,k}$: u 방향 j번 째와 v 방향 k번 째의 컨트롤 포인트가 가지는 가중치

- $\mathbf{P}_{j,k}$: u 방향 j번 째와 v 방향 k번 째의 컨트롤 포인트

- $\mathit{B}^{m}_{j}(u)$: m 차수를 가지는 u 방향의 j번 째 기저 함수

- $\mathit{B}^{n}_{k}(v)$: n 차수를 가지는 v 방향의 k번 째 기저 함수

- $\frac{\sum^{M}_{j = 0}{}\sum^{N}_{k = 0}{w_{j,k}\mathbf{P}_{j,k}\mathit{B}^{m}_{j}(u)\mathit{B}^{n}_{k}(v)}}{\sum^{M}_{j = 0}{}\sum^{N}_{k = 0}{w_{j,k}\mathit{B}^{m}_{j}(u)\mathit{B}^{n}_{k}(v)}}$: u 방향의 기저함수와 가중치를 가지는 컨트롤 포인트를 이용하여 NURBS 곡선을 생성하고, 이 곡선 위의 점을 컨트롤 포인트로 간주하여 다시 v 방향의 기저함수를 이용하여 새로운 NURBS 곡선 위의 점을 생성한다. 이렇게 나온 점들은 NURBS 곡면 위의 점이다.

4. Define the osculating circle.

곡선에 접하면서 곡선을 벗어나지 않는 가장 큰 원을 osculating circle이라 한다.

5. Derive the following curvature equation from a space curve $\mathbf{C}(t)$. Your derivation must be very specific.

$$
\kappa =
\frac{
    \sqrt{
        (z'y'' - y'z'')^2 +
        (x'z'' - z'x'')^2 +
        (x'y'' - y'x'')^2
    }
}
{
    (x'^2 + y'^2 + z'^2)^{3/2}
}
$$

3차원 공간 상의 곡선은 다음과 같이 정의할 수 있다.

$$
\mathbf{C}(t) = [x(t), y(t), z(t)]
$$

이 곡선의 길이를 구하는 식은 다음과 같다.

$$
s = \int{\sqrt{x'^2(t) + y'^2(t) + z'^2(t)}}\mathrm{d}t \\
s = \int{\sqrt{\mathbf{C'}^2(t)}}\mathrm{d}t \\
s = \int{||\mathbf{C'}(t)||}\mathrm{d}t
$$
(1)

이 곡선의 단위 접선 벡터는 다음과 같다.

$$
\mathbf{T}(t) =
\frac{
    \mathbf{C'}
}
{
    ||\mathbf{C'}||
}
$$
(2)

곡률의 정의는 아래와 같고 연쇄 미분 법칙을 사용하면 정의를 (4) 식으로 바꿀 수 있다.

$$
\kappa =
\begin{Vmatrix}
\frac{
    \mathrm{d}\mathbf{T}
}
{
    \mathrm{d}s
}
\end{Vmatrix} =
\begin{Vmatrix}
\frac{
    \mathrm{d}\mathbf{T}
}
{
    \mathrm{d}t
}
\cdot
\frac{
    \mathrm{d}t
}
{
    \mathrm{d}s
}
\end{Vmatrix}
$$

$$
\frac{
    \mathrm{d}\mathbf{T}
}
{
    \mathrm{d}t
} = \mathbf{T'}  \\
\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
} = ||\mathbf{C'}||
$$
(3)

$$
\kappa =
\frac{
    ||\mathbf{T'}||
}
{
    ||\mathbf{C'}||
}
$$
(4)

(2)와 (3) 식을 통해서 곡선의 도함수와 이계도함수를 구할 수 있다.

$$
\mathbf{C'} = ||\mathbf{C'}||\mathbf{T} = 
\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
}\mathbf{T} \\
\mathbf{C''} = 
\frac{
    \mathrm{d}^2s
}
{
    \mathrm{d}t^2
}\mathbf{T} + 
\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
}\mathbf{T'}
$$

도함수와 이계도함수의 외적은 아래와 같다.

$$
\begin{aligned}
\mathbf{C'} \times \mathbf{C''} &= 
\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
}
\frac{
    \mathrm{d}^2s
}
{
    \mathrm{d}t^2
}(\mathbf{T} \times \mathbf{T}) +
(\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
})^2(\mathbf{T} \times \mathbf{T'}) \\
&=
(\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
})^2(\mathbf{T} \times \mathbf{T'}) \quad
(\because \mathbf{T} \times \mathbf{T} = 0)
\end{aligned}   \\
\begin{aligned}
||\mathbf{C'} \times \mathbf{C''}|| &= 
(\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
})^2||\mathbf{T} \times \mathbf{T'}||   \\
&= 
(\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
})^2||\mathbf{T}||||\mathbf{T'}||\sin{\theta}
\end{aligned}
$$

여기서 $\theta$는 $\mathbf{T}$와 $\mathbf{T'}$의 사잇각이고, $\mathbf{T}$가 단위 벡터이기 때문에 $||\mathbf{T}||=1$이다. 또한, $\mathbf{T} \perp \mathbf{T'}$임을 알고 있다. 따라서 도함수와 이계도함수의 외적의 크기는 아래와 같이 표현할 수 있다.

$$
||\mathbf{C'} \times \mathbf{C''}|| = 
(\frac{
    \mathrm{d}s
}
{
    \mathrm{d}t
})^2||\mathbf{T'}||
$$

이를 역으로 탄젠트의 미분을 알 수 있고,

$$
||\mathbf{T'}|| =
\frac{
    ||\mathbf{C'} \times \mathbf{C''}||
}
{
    ||\mathbf{C'}||^2
}
$$
(5)

(5) 식을 (4) 식에 대입하면 곡률 공식을 유도할 수 있다.

$$
\begin{aligned}
\kappa &=
\frac{
    ||\mathbf{T'}||
}
{
    ||\mathbf{C'}||
}   \\
&=
\frac{
    ||\mathbf{C'} \times \mathbf{C''}||
}
{
    ||\mathbf{C'}||^3
}
\end{aligned}
$$
(6)

그리고, 곡선에 대한 식을 위에서 정의하였기 때문에 이를 곡률 공식 (6)에 대입하면 문제에서 정의한 식이 유도 된다.

$$
\mathbf{C}(t) = [x(t), y(t), z(t)]  \\
\mathbf{C'}(t) = [x'(t), y'(t), z'(t)]  \\
\mathbf{C''}(t) = [x''(t), y''(t), z''(t)]
$$

$$
||\mathbf{C'} \times \mathbf{C''}|| = 
\sqrt{
        (z'y'' - y'z'')^2 +
        (x'z'' - z'x'')^2 +
        (x'y'' - y'x'')^2
}
$$

$$
\therefore
\kappa =
\frac{
    \sqrt{
        (z'y'' - y'z'')^2 +
        (x'z'' - z'x'')^2 +
        (x'y'' - y'x'')^2
    }
}
{
    (x'^2 + y'^2 + z'^2)^{3/2}
}
$$
6. Explain the Frenet-Serret frame for a spac curve.

곡선 위의 한 점에서의 단위 접선(tangent, $\mathbf{T}$), 단위 법선(normal, $\mathbf{N}$), 그리고 접선과 법선에 수직한 단위 이중법 벡터(binormal, $\mathbf{B}$), 이 세 단위 벡터를 이용하여 형성된 직교 좌표계를 Frenet-Serret frame이라 한다.

$$
\begin{bmatrix}
    \mathbf{T'}   \\
    \mathbf{N'}   \\
    \mathbf{B'}
\end{bmatrix} = 
\begin{bmatrix}
    0 & \kappa & 0   \\
    -\kappa & 0 & \tau   \\
    0 & -\tau & 0
\end{bmatrix}
\begin{bmatrix}
    \mathbf{T}   \\
    \mathbf{N}   \\
    \mathbf{B}
\end{bmatrix}
$$

여기서, $\kappa$와 $\tau$는 각각 곡률과 비틀림 성분이다.

7. Explain the major difference(s) between the parametric continuity and the geometric continuity

파라메트릭 연속성($\mathbf{C}^n$)은 하나의 곡선에 대해서 연속과 매끄러움을 진단하는 것이고, 지오메트릭 연속성($\mathbf{G}^n$)은 두 개의 곡선에 대해서 연속과 매끄러움을 진단하는 것이다.

8. Summarize the procedure to draw porcupines on a space curve. Explain each step in detail.

삼차원 곡선의 좌표 값을 $\mathbf{C}(t)$라고 하겠다.

- Step 1: $\mathbf{C}(t)$를 곡선의 길이방향($t$)으로 미분하여 탄젠트($\mathbf{T}$)를 구한다. (탄젠트 벡터를 단위 벡터로 만들기 위해서는 정규화 해야한다.)
- Step 2: $\mathbf{T}$를 다시 한번 더 미분하여 노말을 구한다. (이 벡터는 정확한 노말 벡터가 아니다.)
- Step 3: $\mathbf{T}$와 노말을 외적하여 바이노말($\mathbf{B}$)을 구한다. (바이노말 벡터도 정규화한다.)
- Step 4: $\mathbf{B}$와 $\mathbf{T}$를 외적하여 정확한 노말 벡터($\mathbf{N}$)를 구한다. (노말 벡터도 정규화한다.)
- Step 5: 곡률 공식을 이용하여 곡률 값($\kappa$)을 구한다.
- Step 6: 4 단계에서 구한 $\mathbf{N}$ 방향으로 $\kappa$ 길이의 선을 해당 점 위에 그린다.

위와 동일한 순서로 모든 점에서 포큐파인을 그려나가면 3차원 공간 곡선의 곡률 그래프를 그릴 수 있다.