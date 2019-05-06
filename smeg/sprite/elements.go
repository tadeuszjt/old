package sprite

type elements struct {
	length  int
	indices []uint32
	vertexs []vertex
}

func (e *elements) push() {
	i := uint32(len(e.vertexs))
	white := [4]float32{1, 1, 1, 1}
	e.indices = append(e.indices, i, i+1, i+2, i, i+2, i+3)
	e.vertexs = append(e.vertexs,
		vertex{texCoord: vec2{0, 0}, colour: white},
		vertex{texCoord: vec2{1, 0}, colour: white},
		vertex{texCoord: vec2{1, 1}, colour: white},
		vertex{texCoord: vec2{0, 1}, colour: white},
	)

	e.length++
}

func (e *elements) pop() {
	e.indices = e.indices[:len(e.indices)-6]
	e.vertexs = e.vertexs[:len(e.vertexs)-4]
	e.length--
}

func (e *elements) swap(i, j int) {
	i *= 4
	j *= 4
	for k := 0; k < 4; k++ {
		e.vertexs[i], e.vertexs[j] = e.vertexs[j], e.vertexs[i]
		i++
		j++
	}
}
