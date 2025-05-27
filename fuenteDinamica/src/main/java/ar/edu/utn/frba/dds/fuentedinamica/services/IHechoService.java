package ar.edu.utn.frba.dds.fuentedinamica.services;

import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.output.HechoOutputDTO;

import java.util.List;

public interface IHechoService {
  List<HechoOutputDTO> buscarTodos();
  HechoOutputDTO buscarPorId(Long id);
  HechoOutputDTO crear(HechoInputDTO serieInputDTO);
  void eliminar(Long id);
}
