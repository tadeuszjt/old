package sprite

type stack []int

func (s *stack) push(i int) {
	*s = append(*s, i)
}

func (s *stack) pop() int {
	i := len(*s) - 1
	*s = (*s)[:i]
	return i
}

func (s *stack) swap(i, j int) {
	(*s)[i], (*s)[j] = (*s)[j], (*s)[i]
}
