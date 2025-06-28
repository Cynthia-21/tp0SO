package ar.edu.utn.frba.dds.servicioagregador.models.dtos.input;

import ar.edu.utn.frba.dds.servicioagregador.models.dtos.FuenteDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia.CriterioPertenenciaDTO;
import lombok.Data;

import java.util.List;

@Data
public class ColeccionInputDTO {
  private String id;
  private String nombre;
  private String descripcion;
  private List<CriterioPertenenciaDTO> criterios;
  private List<FuenteDTO> fuentes;
}
