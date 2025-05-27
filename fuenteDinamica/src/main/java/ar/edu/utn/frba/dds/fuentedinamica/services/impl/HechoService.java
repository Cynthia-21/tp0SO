package ar.edu.utn.frba.dds.fuentedinamica.services.impl;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.fuentedinamica.models.dtos.output.HechoOutputDTO;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.IHechoRepository;
import ar.edu.utn.frba.dds.fuentedinamica.models.repositories.impl.HechoRepository;
import ar.edu.utn.frba.dds.fuentedinamica.services.IHechoService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class HechoService implements IHechoService {

  private final IHechoRepository hechosRepository;

  public HechoService(HechoRepository hechosRepository) {
    this.hechosRepository = hechosRepository;
  }

  @Override
  public List<HechoOutputDTO> buscarTodos(){
    return this.hechosRepository
        .findAll()
        .stream()
        .map(this::hechoOutputDTO)
        .toList();
  }

  @Override
  public HechoOutputDTO buscarPorId(Long id) {
    var hecho = this.hechosRepository.findById(id);
    return this.hechoOutputDTO(hecho);
  }

  @Override
  public HechoOutputDTO crear(HechoInputDTO hechoInputDTO) {
    var hecho = new Hecho();
    hecho.setTitulo(hechoInputDTO.getTitulo());
    hecho.setDescripcion(hechoInputDTO.getDescripcion());
    this.hechosRepository.save(hecho);

    return this.hechoOutputDTO(hecho);
  }


  @Override
  public void eliminar(Long id) {
    var hecho = this.hechosRepository.findById(id);
    if (hecho != null) {
      this.hechosRepository.delete(hecho);
    }
  }

  private HechoOutputDTO hechoOutputDTO(Hecho hecho) {
    HechoOutputDTO hechoOutputDTO = new HechoOutputDTO();
    hecho.setId(hecho.getId());
    hechoOutputDTO.setTitulo(hecho.getTitulo());
    hechoOutputDTO.setDescripcion(hecho.getDescripcion());
    hechoOutputDTO.setCategorias(hecho.getCategorias());
    hechoOutputDTO.setFechaAcontecimiento(hecho.getFechaAcontecimiento());
    hechoOutputDTO.setFechaDeCarga(hecho.getFechaDeCarga());
    return hechoOutputDTO;
  }
}
